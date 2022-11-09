#include "Renderer.h"

Renderer::Renderer(Shader &shader, int width, int height) : m_shader(shader), m_width(width), m_height(height), m_active_camera(0) {

    m_cameraconstructors.emplace_back([](glm::vec3 position, glm::vec3 up, float yaw, float pitch, float fov, float aspect)->EulerCamera*{
        std::cout << "EulerCamera loaded" << std::endl; return new EulerCamera(position, up, yaw, pitch, fov, aspect);
    });

    m_cameraconstructors.emplace_back([](glm::vec3 position, glm::vec3 up, float yaw, float pitch, float fov, float aspect)->TrackballCamera*{
        std::cout << "TrackballCamera loaded" << std::endl; return new TrackballCamera(position, up, yaw, pitch, fov, aspect);
    });

    m_camera.reset(m_cameraconstructors[0](glm::vec3(0.0f,0.0f,5.0f), glm::vec3(0.0,1.0,0.0), -90.0, 0, 45.0, (float)m_width/(float)m_height));

}

Renderer::~Renderer() {
    for(auto d : m_drawables)
        delete d;
    /*
    for(auto l : m_lights)
        delete l;
    */
    m_shader.deleteProg();
    m_cameraconstructors.clear();
    m_camera.reset(nullptr);
}

void Renderer::Draw() {
    for(Drawable *drawable : m_drawables){
        m_shader.useShaderProgram();
        m_shader.setMat4("projection", m_camera->projectionmatrix());
        m_shader.setMat4("view", m_camera->viewmatrix());
        m_shader.setVec3("viewPos", m_camera->position());
        /*
        m_shader.setBool("normalmode", m_normal_mode);
        m_shader.setBool("Texcoordmode", m_Texcoord_mode);
        for(int i = 0; i < (int) m_lights.size(); i++){
            std::string str_i = std::to_string(i);
            m_shader.setVec4("lights["+str_i+"].vec", m_lights[i]->getVec());
            m_shader.setVec3("lights["+str_i+"].color", m_lights[i]->getColor());
        }
        */

        drawable->Draw(m_shader);
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
}

void Renderer::add_Drawable(Drawable *drawable) {
    m_drawables.emplace_back(drawable);
}

void Renderer::change_Camera(){
    unsigned int cam_size = m_cameraconstructors.size();
    if(m_active_camera < cam_size){
        m_active_camera = (m_active_camera+1)%cam_size;
        m_camera.reset(m_cameraconstructors[m_active_camera](glm::vec3(0.0,0.0f,10.0f), glm::vec3(0.0,1.0,0.0), -90.0, 0, 45, (float)m_width/(float)m_height));
    }
}

Camera* Renderer::get_Camera() {
    return m_camera.get();
}

const std::vector<Drawable *> &Renderer::getMDrawables() const {
    return m_drawables;
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

