#ifndef TPM2_GEOM_RENDERER_H
#define TPM2_GEOM_RENDERER_H

#include <memory>
#include <functional>
#include "Shader.h"
#include "Drawable.h"
#include "Camera.h"

class Renderer {

public:
    Renderer(Shader &shader, int width, int height);
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

private:
    std::vector<Drawable*> m_drawables;
    //std::vector<Light*> m_lights;
    Shader m_shader;

    int m_width;
    int m_height;

    std::unique_ptr<Camera> m_camera;
    using cameraconstructor=std::function<Camera*(glm::vec3, glm::vec3, float, float, float, float)>;
    std::vector<cameraconstructor> m_cameraconstructors;
    unsigned int m_active_camera;
};


#endif //TPM2_GEOM_RENDERER_H
