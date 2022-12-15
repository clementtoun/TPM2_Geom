#include "Renderer.h"
#include "TriMesh.h"
#include "AxisMesh.h"
#include "../libs/glad/glad.h"

Renderer::Renderer(Shader &shader, Shader &shaderP, int width, int height) : m_shader(shader), m_shaderP(shaderP), m_width(width), m_height(height), m_active_camera(0) {

    m_shaderC = Shader();
    m_shaderC.attachShader("../src/Shader/colorV.shader", GL_VERTEX_SHADER);
    m_shaderC.attachShader("../src/Shader/colorF.shader", GL_FRAGMENT_SHADER);
    m_shaderC.linkProgram();

    m_shaderSC = Shader();
    m_shaderSC.attachShader("../src/Shader/simpleColorV.shader", GL_VERTEX_SHADER);
    m_shaderSC.attachShader("../src/Shader/simpleColorF.shader", GL_FRAGMENT_SHADER);
    m_shaderSC.linkProgram();

    m_axisMesh = new AxisMesh();

    m_cameraconstructors.emplace_back([](glm::vec3 position, glm::vec3 up, float yaw, float pitch, float fov, float aspect)->EulerCamera*{
        std::cout << "EulerCamera loaded" << std::endl; return new EulerCamera(position, up, yaw, pitch, fov, aspect);
    });

    m_cameraconstructors.emplace_back([](glm::vec3 position, glm::vec3 up, float yaw, float pitch, float fov, float aspect)->TrackballCamera*{
        std::cout << "TrackballCamera loaded" << std::endl; return new TrackballCamera(position, up, yaw, pitch, fov, aspect);
    });

    m_camera.reset(m_cameraconstructors[0](glm::vec3(0.f,0.f,2.f), glm::vec3(0.f,1.f,0.f), -90.f, 0.f, 45.f, (float)m_width/(float)m_height));

    m_camera->setviewport(glm::vec4(0, 0, m_width, m_height));
}

Renderer::~Renderer() {
    for(auto d : m_drawables)
        delete d;
    delete m_axisMesh;
    m_shader.deleteProg();
    m_shaderC.deleteProg();
    m_cameraconstructors.clear();
    m_camera.reset(nullptr);
}

void Renderer::Draw() {

    glStencilMask(0x00);
    m_shaderC.useShaderProgram();
    m_shaderC.setMat4("projection", m_camera->projectionmatrix());
    m_shaderC.setMat4("view", m_camera->viewmatrix());
    m_axisMesh->Draw(m_shaderC, GL_LINES);

    for(Drawable *drawable : m_drawables) {

        if(drawable == m_selected_mesh) {
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
        }
        else{
            glStencilMask(0x00);
        }

        m_shader.useShaderProgram();
        m_shader.setMat4("projection", m_camera->projectionmatrix());
        m_shader.setMat4("view", m_camera->viewmatrix());
        m_shader.setVec3("viewPos", m_camera->position());
        m_shader.setVec2("WIN_SCALE", glm::vec2(m_width, m_height));
        m_shader.setBool("drawline", m_drawLinePoint);
        m_shader.setBool("f_mode", m_show_f_mode);
        drawable->Draw(m_shader, GL_TRIANGLES);

        if(m_drawLinePoint) {
            m_shaderP.useShaderProgram();
            m_shaderP.setMat4("projection", m_camera->projectionmatrix());
            m_shaderP.setMat4("view", m_camera->viewmatrix());
            drawable->Draw(m_shaderP, GL_POINTS);
        }

        if(drawable == m_selected_mesh){
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            m_shaderSC.useShaderProgram();
            m_shaderSC.setMat4("projection", m_camera->projectionmatrix());
            m_shaderSC.setMat4("view", m_camera->viewmatrix());
            m_shaderSC.setVec3("color", glm::vec3(0.0f, 0.3f, 1.f));
            drawable->scale(glm::vec3(1.1f));
            drawable->Draw(m_shaderSC, GL_TRIANGLES);
            drawable->scale(glm::vec3(1.f/1.1f));
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
        }
    }


    /*
    for(auto light : m_lights){
        m_lightShader.use();
        m_lightShader.setMat4("projection", m_camera->projectionmatrix());
        m_lightShader.setMat4("view", m_camera->viewmatrix());
        light->Draw(m_lightShader);
    }
    */
}

void Renderer::resize(int width, int height) {
    m_width = width;
    m_height = height;
    m_camera->setAspect((float) width / (float) height);
    m_camera->setviewport(glm::vec4(0, 0, m_width, m_height));
}

void Renderer::add_Drawable(Drawable *drawable) {
    m_drawables.emplace_back(drawable);
}

void Renderer::change_Camera(){
    unsigned int cam_size = m_cameraconstructors.size();
    if(m_active_camera < cam_size){
        m_active_camera = (m_active_camera+1)%cam_size;
        m_camera.reset(m_cameraconstructors[m_active_camera](glm::vec3(0.f,0.f,2.f), glm::vec3(0.f,1.f,0.f), -90.f, 0.f, 45.f, (float)m_width/(float)m_height));
        m_camera->setviewport(glm::vec4(0, 0, m_width, m_height));
    }
}

Camera* Renderer::get_Camera() {
    return m_camera.get();
}

const std::vector<Drawable *> &Renderer::getMDrawables() const {
    return m_drawables;
}

void Renderer::toggleDrawLinePoint() {
    m_drawLinePoint = !m_drawLinePoint;
}

Ray Renderer::getRayFromMousePos(double xpos, double ypos) {
    auto trans = glm::inverse(m_camera->projectionmatrix() * m_camera->viewmatrix());
    auto near = glm::vec4(xpos / m_width * 2.f - 1.f, (1.f - ypos / m_height) * 2.f - 1.f, -1.0f, 1.0f);
    auto far = glm::vec4(near.x, near.y, 1.0f, 1.0f);
    near = trans * near;
    far = trans * far;
    near /= near.w;
    far /= far.w;
    auto dir = glm::normalize(far - near);
    return {near, dir, -1};
}

int Renderer::intersectPointMesh(double xpos, double ypos) {

    Ray r = getRayFromMousePos(xpos, ypos);

    int index = -1;

    for(Drawable *drawable : m_drawables) {
        auto *tm = (TriMesh*) drawable;
        index = tm->intersectPoint(r);
    }

    return index;
}

void Renderer::intersectMesh(double xpos, double ypos) {
    Ray r = getRayFromMousePos(xpos, ypos);

    float tmin = 100000.0f;
    m_selected_mesh = nullptr;

    for(Drawable *drawable : m_drawables) {
        auto *tm = (TriMesh*) drawable;
        AABB tmAABB = tm->getAABB();
        glm::mat4 model = tm->getModel();
        auto min = model * glm::vec4(tmAABB.min, 1.f);
        auto max = model * glm::vec4(tmAABB.max, 1.f);
        tmAABB.min = min / min.w;
        tmAABB.max = max / max.w;
        bool intersect = intersectBox(tmAABB, r);
        if(intersect && r.t < tmin){
            m_selected_mesh = drawable;
            tmin = r.t;
        }
    }
}

void Renderer::toggleShowf() {
    m_show_f_mode = !m_show_f_mode;
}

Drawable *Renderer::getSelectedMesh() const {
    return m_selected_mesh;
}

/*
void Renderer::add_Light(Light *light) {
    m_lights.emplace_back(light);
}


void Renderer::toggleNormalmode() {
    if(m_Texcoord_mode)
        m_Texcoord_mode = false;
    m_normal_mode = !m_normal_mode;
}

void Renderer::toggleTexcoordmode() {
    if(m_normal_mode)
        m_normal_mode = false;
    m_Texcoord_mode = !m_Texcoord_mode;
}
*/

