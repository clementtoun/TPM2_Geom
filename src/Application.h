#ifndef TPM2_GEOM_APPLICATION_H
#define TPM2_GEOM_APPLICATION_H

#include "../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"

class Application {

public:
    Application(const char* name, int width, int height);

    ~Application();

    void Run();

private:
    virtual void Init() = 0;
    virtual void mainloop() = 0;

protected:
    virtual void processInput(GLFWwindow *window) {};
    virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {};
    virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos) {};
    virtual void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    int m_width, m_height;
    float m_deltaTime, m_lastFrame;
    GLFWwindow* m_window;
};


#endif //TPM2_GEOM_APPLICATION_H
