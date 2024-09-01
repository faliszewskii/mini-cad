//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_CAMERA_H
#define OPENGL_SANDBOX_CAMERA_H


#include "glad/glad.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <map>

#include "../SceneNode.h"

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
const float SPEED = 1.f;
const float SENSITIVITY = 0.01f;
const float ZOOM_SENSITIVITY = 0.1f;

class Camera {
private:
    CameraMode cameraType;

    glm::quat orientation;

    float movementSpeed;
    float mouseSensitivity;
    float zoomSensitivity; // TODO Maybe change based on how close anchor to
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

public:
    glm::vec3 position;
    glm::vec3 anchor;
    float nearPlane;
    float farPlane;
    float fov;

    bool stereoscopicVision = false;
    float stereoscopicIOD = 0.068;
    float stereoscopicDistance = 9;

    std::map<CameraMode, void (Camera::*)(float, float)> mouseHandlerMapping{
            {CameraMode::FREE,   &Camera::processMouseMovementFree},
            {CameraMode::ANCHOR, &Camera::processMouseMovementAnchor}
    };
    std::map<CameraMode, void (Camera::*)(CameraMovement direction, float)> keyboardHandlerMapping{
            {CameraMode::FREE,   &Camera::processKeyboardFree},
            {CameraMode::ANCHOR, &Camera::processKeyboardAnchor}
    };

    explicit Camera(
            int screenWidth,
            int screenHeight,
            CameraMode cameraMode = FREE,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 anchor = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat orientation = glm::quat(1, 0, 0, 0)
    );

    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] glm::mat4 getProjectionMatrix() const;
    [[nodiscard]] glm::vec3 getViewPosition() const;

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);

    int screenWidth;
    int screenHeight;

    glm::mat4 getViewMatrixStereo(bool left) const;

    glm::vec3 getViewPositionStereo(bool left) const;

    glm::mat4 getProjectionMatrixStereo(bool left);

private:

    void processMouseMovementFree(float xoffset, float yoffset);
    void processMouseMovementAnchor(float xoffset, float yoffset);

    void processKeyboardFree(CameraMovement direction, float deltaTime);
    void processKeyboardAnchor(CameraMovement direction, float deltaTime);

    void updateDirections();

    static glm::mat4 myFrustrum(float left, float right, float bottom, float top, float zNear, float zFar);
    glm::mat4 myProjectionStereo(float yFov, float aspectRatio, float zNear, float zFar, float iod, float distance, bool leftEye);
};


#endif //OPENGL_SANDBOX_CAMERA_H
