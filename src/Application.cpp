#include "Application.h"

#include <iostream>
#include <string>

Application::Application(const char *name, int width, int height) : m_width(width), m_height(height) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(m_window);

    glfwSetWindowUserPointer(m_window, this);

    auto mousefunc = [](GLFWwindow* w, double xpos, double ypos)
    {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->mouse_callback(w, xpos, ypos);
    };

    auto framefunc = [](GLFWwindow *w, int width, int height)
    {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->framebuffer_size_callback(w, width, height);
    };

    auto keyfunc = [](GLFWwindow* w, int key, int scancode, int action, int mods)
    {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->key_callback(w, key, scancode, action, mods);
    };

    glfwSetFramebufferSizeCallback(m_window, framefunc);
    glfwSetCursorPosCallback(m_window, mousefunc);
    glfwSetKeyCallback(m_window, keyfunc);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(m_window, (mode->width-width)/2, (mode->height-height)/2);

    glfwShowWindow(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glViewport(0, 0, width, height);
}

Application::~Application() {
    glfwTerminate();
}

void Application::Run() {
    Init();

    while(!glfwWindowShouldClose(m_window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

        std::cout << int(1.0 / m_deltaTime) << " fps" << std::endl;

        mainloop();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Application::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
