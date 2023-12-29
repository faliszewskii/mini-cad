#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum CameraType {
    FREE,
    ANCHOR
};

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.01f;
const float ZOOM        =  45.0f;


class Camera
{
public:
    CameraType cameraType;
    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    glm::vec3 anchor;
    // euler Angles
    float yaw;
    float pitch;
    // camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    Camera(
            CameraType cameraType = FREE,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3 anchor = glm::vec3(0.0f, 0.0f, 0.0f),
            float yaw = YAW, float pitch = PITCH);
    glm::mat4 getViewMatrix();
    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);
private:
    void processMouseMovementFree(float xoffset, float yoffset);
    void processMouseMovementAnchor(float xoffset, float yoffset);
};
#endif