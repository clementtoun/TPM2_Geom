#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    LEFT = 0,
    FORWARD,
    RIGHT,
    BACKWARD,
    UP,
    DOWN
};

constexpr float FOV      =  45.0f;
constexpr float ASPECT     =  16.0f/9.0f;
constexpr float NEAR      =  0.01f;
constexpr float FAR     =  100.0f;

/*------------------------------------------------------------------------------------------------------------------------*/
/*                                                Abstract Camera                                                         */
/*------------------------------------------------------------------------------------------------------------------------*/

class Camera {

public:

    explicit Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 1.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), glm::vec3 look = glm::vec3(0.f, 0.f, 0.f),
                    float fov=FOV, float aspect=ASPECT, float zoom=45.f);

    virtual ~Camera();

    virtual // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 viewmatrix() const;
    glm::mat4 projectionmatrix() const;
    float &zoom();
    glm::vec3 &position();
    void setposition(glm::vec3 position);
    void setAspect(float aspect);

    void setviewport(glm::vec4 viewport);

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    virtual void processkeyboard(Camera_Movement direction, float deltaTime) {};
    // Processes input received from a mouse input system.
    virtual void processmouseclick(int button, float xpos, float ypos);
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    virtual void processmousemovement(int button, float xpos, float ypos) {};
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    virtual void processmousescroll(float yoffset);

protected:

    float _fov;
    float _aspect;
    float _near;
    float _far;
    glm::mat4 _projection{};

    glm::vec3 _position{};
    glm::vec3 _front{};
    glm::vec3 _up{};
    float _zoom;

    glm::vec4 _viewport{};

    // mouse movement
    int _mousebutton{};
    float _mousestartx{};
    float _mousestarty{};

    // Camera options
    float _movementspeed{};
    float _mousesensitivity{};
};


/*------------------------------------------------------------------------------------------------------------------------*/
/*                                       Euler Camera : Yaw, Pitch, Roll                                                  */
/*------------------------------------------------------------------------------------------------------------------------*/

// from learnopenGL tutorial
// Default camera values
constexpr float YAW        = -90.0f;
constexpr float PITCH      =  0.0f;
constexpr float SPEED      =  2.0f;
constexpr float SENSITIVTY =  0.05f;
constexpr float ZOOM       =  45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class EulerCamera : public Camera {

public:

    // Constructor with vectors (default constructor)
    explicit EulerCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float fov = FOV, float aspect = ASPECT);

    ~EulerCamera() override;

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processkeyboard(Camera_Movement direction, float deltaTime) override;

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processmousemovement(int button, float xpos, float ypos) override;

    void processmousescroll(float delta) override;

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updatecameravectors();

    // Camera Attributes
    glm::vec3 _right{};
    glm::vec3 _worldup{};
    // Eular Angles
    float _yaw;
    float _pitch;
};


/*------------------------------------------------------------------------------------------------------------------------*/
/*                                            Trackball Camera                                                            */
/*                                  Always rotate around point (0, 0, 0)                                                  */
/*                                      Accumulate error on roll                                                          */
/*------------------------------------------------------------------------------------------------------------------------*/
class TrackballCamera : public Camera {

public:

    // Constructor with vectors (default constructor)
    explicit TrackballCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float fov = FOV, float aspect = ASPECT);

    ~TrackballCamera() override;

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processkeyboard(Camera_Movement direction, float deltaTime) override;

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processmousemovement(int button, float xpos, float ypos) override;

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updatecameravectors();

    // Camera Attributes
    glm::vec3 _right{};
    glm::vec3 _worldup{};
    // Eular Angles
    float _yaw;
    float _pitch;
};

#endif // CAMERA_H
