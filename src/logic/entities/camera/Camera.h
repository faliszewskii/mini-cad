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
const float SPEED = 2.5f;
const float SENSITIVITY = 0.01f;
const float ZOOM_SENSITIVITY = 0.1f;

class Camera {
private:
    CameraMode cameraType;

    glm::qua<double> orientation;

    float movementSpeed;
    float mouseSensitivity;
    float zoomSensitivity; // TODO Maybe change based on how close anchor to
    glm::vec<3, double> front;
    glm::vec<3, double> up;
    glm::vec<3, double> right;

public:
    glm::vec<3, double> position;
    glm::vec<3, double> anchor;
    float nearPlane;
    float farPlane;
    float fov;

    std::map<CameraMode, void (Camera::*)(double, double)> mouseHandlerMapping{
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
            glm::vec<3, double> position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec<3, double> anchor = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::qua<double> orientation = glm::quat(1, 0, 0, 0)
    );

    [[nodiscard]] glm::mat<4,4,double> getViewMatrix() const;
    [[nodiscard]] glm::mat<4,4,double> getProjectionMatrix() const;
    [[nodiscard]] glm::vec<3, double> getViewPosition() const;

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(double xoffset, double yoffset);
    void processMouseScroll(double yoffset);

    int screenWidth;
    int screenHeight;
private:

    void processMouseMovementFree(double xoffset, double yoffset);
    void processMouseMovementAnchor(double xoffset, double yoffset);

    void processKeyboardFree(CameraMovement direction, float deltaTime);
    void processKeyboardAnchor(CameraMovement direction, float deltaTime);

    void updateDirections();
};


#endif //OPENGL_SANDBOX_CAMERA_H
