#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <map>

enum CameraModeOld {
    FREE,
    ANCHOR
};

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovementOld {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float SPEEDOld       =  2.5f;
const float SENSITIVITYOld =  0.01f;
const float ZOOM_SENSITIVITYOld =  0.1f;


class CameraOld
{
private:
    CameraModeOld cameraType;

    glm::quat orientation;
    glm::vec3 position;
    glm::vec3 anchor;

    float movementSpeed;
    float mouseSensitivity;
    float zoomSensitivity; // TODO Maybe change based on how close anchor to
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

public:
    std::map<CameraModeOld, void (CameraOld::*)(float, float)> mouseHandlerMapping {
            {CameraModeOld::FREE,   &CameraOld::processMouseMovementFree},
            {CameraModeOld::ANCHOR, &CameraOld::processMouseMovementAnchor}
    };
    std::map<CameraModeOld, void (CameraOld::*)(CameraMovementOld direction, float)> keyboardHandlerMapping {
            {CameraModeOld::FREE,   &CameraOld::processKeyboardFree},
            {CameraModeOld::ANCHOR, &CameraOld::processKeyboardAnchor}
    };

    explicit CameraOld(
            CameraModeOld cameraMode = FREE,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 anchor = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat orientation = glm::quat(1,0,0,0)
            );


    glm::mat4 getViewMatrix();

    void processKeyboard(CameraMovementOld direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);

private:

    void processMouseMovementFree(float xoffset, float yoffset);
    void processMouseMovementAnchor(float xoffset, float yoffset);
    void processKeyboardFree(CameraMovementOld direction, float deltaTime);
    void processKeyboardAnchor(CameraMovementOld direction, float deltaTime);

    void updateDirections();
};
#endif