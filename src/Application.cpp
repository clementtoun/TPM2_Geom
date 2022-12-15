#include "Application.h"

#include <iostream>
#include <string>

void error_callback(int error, const char *description) {
    std::cout << "Error: " << description << std::endl;
}

Application::Application(const char *name, int width, int height) : m_width(width), m_height(height) {

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

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
        exit(EXIT_FAILURE);
    }

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

    auto scrollfunc = [](GLFWwindow* w, double xoffset, double yoffset)
    {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->scroll_callback(w, xoffset, yoffset);
    };

    auto clicfunc = [](GLFWwindow* w, int button, int action, int mods)
    {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->mouse_button_callback(w, button, action, mods);
    };

    auto centerfunc = [](GLFWwindow* w, int enter)
    {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->cursor_enter_callback(w, enter);
    };

    glfwSetCursorEnterCallback(m_window, centerfunc);
    glfwSetFramebufferSizeCallback(m_window, framefunc);
    glfwSetCursorPosCallback(m_window, mousefunc);
    glfwSetKeyCallback(m_window, keyfunc);
    glfwSetScrollCallback(m_window, scrollfunc);
    glfwSetMouseButtonCallback(m_window, clicfunc);
    glfwSetWindowUserPointer(m_window, this);
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(m_window, (mode->width-width)/2, (mode->height-height)/2);

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(-1);

    glfwShowWindow(m_window);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glfwGetFramebufferSize(m_window, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);
}

Application::~Application() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::Run() {
    Init();

    glfwPollEvents();
    mainloop();
    glfwSwapBuffers(m_window);

    while(!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        if (m_cursor_in) {
            float currentFrame = static_cast<float>(glfwGetTime());
            m_deltaTime = currentFrame - m_lastFrame;
            m_lastFrame = currentFrame;

            //std::cout << int(1.0 / m_deltaTime) << " fps" << std::endl;

            mainloop();

            glfwSwapBuffers(m_window);
        }
    }
}

void Application::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    m_width = width;
    m_height = height;
}

void Application::cursor_enter_callback(GLFWwindow *window, int enter) {
    m_cursor_in = enter;
}
