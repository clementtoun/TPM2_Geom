#ifndef TPM2_GEOM_MYAPP_H
#define TPM2_GEOM_MYAPP_H

#include "Application.h"
#include "Renderer.h"
#include "mesh_fonction_propagator.h"

class myApp : public Application {
public:
    myApp(const char *name, int width, int height) : Application(name, width, height) {}

    ~myApp();
    void mainloop() override;

private:
    void Init() override;

    void processInput(GLFWwindow *window) override;
    void mouse_callback(GLFWwindow *window, double xpos, double ypos) override;
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) override;
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) override;

protected:
    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) override;

private:
    bool m_drawfill = true;
    bool cursor_enable = true;

    mesh_fonction_propagator* m_mfp;
    int m_mesh_propagator_state = 0;
    bool m_ready_propagate = false;

    Renderer* m_renderer;
};


#endif //TPM2_GEOM_MYAPP_H
