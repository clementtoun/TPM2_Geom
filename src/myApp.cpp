#include "myApp.h"
#include "TriMesh.h"
#include "OpenMeshUtils.h"

void myApp::Init() {

    Shader shader = Shader();
    shader.attachShader("../src/Shader/simpleV.shader", GL_VERTEX_SHADER);
    shader.attachShader("../src/Shader/simpleG.shader", GL_GEOMETRY_SHADER);
    shader.attachShader("../src/Shader/simpleF.shader", GL_FRAGMENT_SHADER);
    shader.linkProgram();

    Shader shaderP = Shader();
    shaderP.attachShader("../src/Shader/pointV.shader", GL_VERTEX_SHADER);
    shaderP.attachShader("../src/Shader/pointF.shader", GL_FRAGMENT_SHADER);
    shaderP.linkProgram();

    m_mfp = new mesh_fonction_propagator();

    m_renderer = new Renderer(shader, shaderP, m_width, m_height);

    auto *mesh = new TriMesh();
    mesh->readMeshFromFile("../obj/bunny.obj");
    //mesh->rotate(-90.f, glm::vec3(1.f, 0.f, 0.f));
    mesh->normalize_center();
    m_renderer->add_Drawable(mesh);

    /*
    auto *mesh2 = new TriMesh();
    mesh2->readMeshFromFile("../obj/roller.obj");
    mesh2->translate(glm::vec3(0.0f,0.0f,-15.0f), 0);
    m_renderer->add_Drawable(mesh2);
    */
}

void myApp::mainloop() {
    //std::cout << 1/m_deltaTime << std::endl;

    processInput(m_window);

    glClearColor(0.26, 0.26, 0.26, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (m_drawfill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_renderer->Draw();
}

myApp::~myApp() {
    delete m_renderer;
    delete m_mfp;
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
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
        auto mesh = (TriMesh*) m_renderer->getMDrawables().front();
        laplacianSmoothing(mesh->getMesh(), 1.f);
        mesh->commitChange();
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && m_ready_propagate){
        TriMesh* m = (TriMesh*) m_renderer->getSelectedMesh();
        if(m != nullptr) {
            m_mfp->propagate();
            m->commitChange();
        }
    }
}

void myApp::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    m_renderer->get_Camera()->processmousemovement(state, (float) xpos, (float) ypos);
    if (state == GLFW_PRESS) {
        if (cursor_enable) {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursor_enable = !cursor_enable;
        }
    }
    else {
        if (!cursor_enable) {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursor_enable = !cursor_enable;
        }
    }
}

void myApp::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    Application::framebuffer_size_callback(window, width, height);
    m_renderer->resize(width, height);
}

void myApp::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        m_drawfill = !m_drawfill;
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        auto mesh = (TriMesh*) m_renderer->getMDrawables().front();
        laplacianSmoothing(mesh->getMesh(), 1.f);
        mesh->commitChange();
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        m_renderer->change_Camera();
    }
    if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS){
        TriMesh* m = (TriMesh*) m_renderer->getSelectedMesh();
        if(m != nullptr)
            m->deform(0.01);
    }
    if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS){
        TriMesh* m = (TriMesh*) m_renderer->getSelectedMesh();
        if(m != nullptr)
            m->deform(-0.01);
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        TriMesh* m = (TriMesh*) m_renderer->getSelectedMesh();
        switch (m_mesh_propagator_state) {
            case 0: {
                if(m != nullptr){
                    m_ready_propagate = false;
                    m_renderer->toggleDrawLinePoint();
                    m_mfp->setMesh(m->getMesh());
                    m_mesh_propagator_state++;
                }
                else{
                    std::cout << "No mesh selected" << std::endl;
                }
                break;
            }
            case 1: {
                m_mfp->set_zero_index();
                m->commitChange();
                m_mesh_propagator_state++;
                break;
            }
            case 2: {
                m_mfp->set_one_index();
                m->commitChange();
                m_mesh_propagator_state = 0;
                m_renderer->toggleDrawLinePoint();
                m_ready_propagate = true;
                break;
            }
            default:
                std::cout << "Bug !" << std::endl;
        }
    }
    if (key == GLFW_KEY_I && action == GLFW_PRESS && m_ready_propagate) {
        TriMesh* m = (TriMesh*) m_renderer->getSelectedMesh();
        if(m != nullptr) {
            m_mfp->propagate();
            m->commitChange();
        }
    }
    if (key == GLFW_KEY_U && action == GLFW_PRESS && m_ready_propagate) {
        TriMesh* m = (TriMesh*) m_renderer->getSelectedMesh();
        if(m != nullptr) {
            m_mfp->propagateMatrix();
            m->commitChange();
        }
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        m_renderer->toggleShowf();
    }
}

void myApp::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    m_renderer->get_Camera()->processmousescroll(float(yoffset));
}

void myApp::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        auto cam = m_renderer->get_Camera();
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        cam->processmouseclick(button, float(xpos), float(ypos));
        if(m_mesh_propagator_state == 0)
            m_renderer->intersectMesh(xpos, ypos);
        else{
            m_renderer->intersectPointMesh(xpos, ypos);
        }
    }
}
