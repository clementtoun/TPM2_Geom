#ifndef TPM2_GEOM_RENDERER_H
#define TPM2_GEOM_RENDERER_H

#include <memory>
#include <functional>
#include "Shader.h"
#include "Drawable.h"
#include "Camera.h"
#include "RayUtils.h"

class Renderer {

public:
    Renderer(Shader &shader, Shader &shaderP, int width, int height);
    ~Renderer();
    void resize(int width, int height);
    void Draw();
    void add_Drawable(Drawable *drawable);
    //void add_Light(Light *light);
    void change_Camera();
    Camera * get_Camera();
    //void toggleNormalmode();
    //void toggleTexcoordmode();
    const std::vector<Drawable*> &getMDrawables() const;

    Drawable *getSelectedMesh() const;

    int intersectPointMesh(double xpos, double ypos);
    void intersectMesh(double xpos, double ypos);
    void toggleDrawLinePoint();
    void toggleShowf();

private:
    Ray getRayFromMousePos(double xpos, double ypos);

    Drawable* m_selected_mesh = nullptr;

    std::vector<Drawable*> m_drawables;
    Drawable* m_axisMesh;
    //std::vector<Light*> m_lights;
    Shader m_shader;
    Shader m_shaderP;
    Shader m_shaderC;
    Shader m_shaderSC;

    int m_width;
    int m_height;

    std::unique_ptr<Camera> m_camera;
    using cameraconstructor=std::function<Camera*(glm::vec3, glm::vec3, float, float, float, float)>;
    std::vector<cameraconstructor> m_cameraconstructors;
    unsigned int m_active_camera;

    bool m_drawLinePoint = false;
    bool m_show_f_mode = false;
};


#endif //TPM2_GEOM_RENDERER_H
