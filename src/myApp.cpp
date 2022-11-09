#include "myApp.h"
#include "TriMesh.h"
#include "OpenMeshUtils.h"

float vertices[] = {
        // positions         // colors
        0.5f, -0.5f, -5.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,  // bottom left
        0.0f,  0.5f, -5.0f,  0.0f, 0.0f, 1.0f   // top
};

void myApp::Init() {

    Shader shader = Shader("../src/Shader/simpleV.shader", "../src/Shader/simpleF.shader");

    m_renderer = new Renderer(shader, m_width, m_height);

    auto *mesh = new TriMesh();
    mesh->readMeshFromFile("../obj/Skull.obj");
    mesh->rotate(-90, glm::vec3(1.0f, 0.0f, 0.0f), 0);
    mesh->translate(glm::vec3(0.0f,40.0f,-12.0f), 0);

    m_renderer->add_Drawable(mesh);
}

void myApp::mainloop() {
    //std::cout << 1/m_deltaTime << std::endl;

    processInput(m_window);

    glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_drawfill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_renderer->Draw();
}

myApp::~myApp() {
    delete m_renderer;
}

void myApp::processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    auto camera = m_renderer->get_Camera();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processkeyboard(FORWARD, m_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processkeyboard(LEFT, m_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processkeyboard(BACKWARD, m_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processkeyboard(RIGHT, m_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processkeyboard(UP, m_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->processkeyboard(DOWN, m_deltaTime);
}

void myApp::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (!m_cursor_enable)
        m_renderer->get_Camera()->processmousemovement(0, (float)xpos, (float) ypos);
}

void myApp::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    Application::framebuffer_size_callback(window, width, height);
    m_width = width;
    m_height = height;
    m_renderer->resize(m_width, m_height);
}

void myApp::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        m_drawfill = !m_drawfill;
    if (key == GLFW_KEY_F10 && action == GLFW_PRESS) {
        m_cursor_enable = !m_cursor_enable;
        if (m_cursor_enable) {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
        }
        else {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        auto mesh = (TriMesh*) m_renderer->getMDrawables().front();
        laplacianSmoothing(mesh->getMMesh(), 0.5f);
        mesh->commitChange();
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        m_renderer->change_Camera();
    }
}
