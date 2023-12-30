#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <map>

enum CameraMode {
    FREE,
    ANCHOR
};

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
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
private:
    CameraMode cameraType;

    glm::quat orientation;
    glm::vec3 position;
    glm::vec3 anchor;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

public:
    std::map<CameraMode, void (Camera::*)(float, float)> mouseHandlerMapping {
            {CameraMode::FREE,   &Camera::processMouseMovementFree},
            {CameraMode::ANCHOR, &Camera::processMouseMovementAnchor}
    };
    std::map<CameraMode, void (Camera::*)(CameraMovement direction, float)> keyboardHandlerMapping {
            {CameraMode::FREE,   &Camera::processKeyboardFree},
            {CameraMode::ANCHOR, &Camera::processKeyboardAnchor}
    };

    explicit Camera(
            CameraMode cameraMode = FREE,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 anchor = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat orientation = glm::quat(1,0,0,0)
            );


    glm::mat4 getViewMatrix();

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);

private:

    void processMouseMovementFree(float xoffset, float yoffset);
    void processMouseMovementAnchor(float xoffset, float yoffset);
    void processKeyboardFree(CameraMovement direction, float deltaTime);
    void processKeyboardAnchor(CameraMovement direction, float deltaTime);

    void updateDirections();
};
#endif