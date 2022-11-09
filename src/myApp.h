#ifndef TPM2_GEOM_MYAPP_H
#define TPM2_GEOM_MYAPP_H

#include "Application.h"
#include "Renderer.h"

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

    bool m_drawfill = true;
    bool m_cursor_enable = false;

    Renderer* m_renderer;
};


#endif //TPM2_GEOM_MYAPP_H
