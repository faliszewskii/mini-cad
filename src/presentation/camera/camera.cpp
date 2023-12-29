#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include "camera.h"

Camera::Camera(CameraType cameraType, glm::vec3 position, glm::vec3 worldUp, glm::vec3 anchor, float yaw, float pitch) :
    cameraType(cameraType),
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
    this->position = position;
    this->anchor = anchor;
    this->worldUp = worldUp;
    this->up = worldUp;
    this->yaw = yaw;
    this->pitch = pitch;
    right = glm::normalize(glm::cross(front, up));
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    switch(cameraType) {
        case CameraType::FREE:
            processMouseMovementFree(xoffset, yoffset);
            break;
        case CameraType::ANCHOR:
            processMouseMovementAnchor(xoffset, yoffset);
            break;
    }
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovementAnchor(float xoffset, float yoffset)
{
    // TODO Move by anchor
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    float hAngle = -yoffset / 2;
    glm::vec3 axis = right;
    float w = cos(hAngle);
    float x = axis.x * sin(hAngle);
    float y = axis.y * sin(hAngle);
    float z = axis.z * sin(hAngle);
    glm::quat rotationY(w, x, y, z);
    glm::mat4 rotationMatrixY = glm::mat4_cast(rotationY);

    hAngle = xoffset / 2;
    axis = up;
    w = cos(hAngle);
    x = axis.x * sin(hAngle);
    y = axis.y * sin(hAngle);
    z = axis.z * sin(hAngle);
    glm::quat rotationX(w, x, y, z);
    glm::mat4 rotationMatrixX = glm::mat4_cast(rotationX);

    position = rotationMatrixX * rotationMatrixY * glm::vec4(position, 1);
    front = glm::normalize(anchor - position);
    
    up = glm::normalize(glm::vec3(rotationMatrixY * glm::vec4(up + position - anchor, 1)) - position + anchor);
    right = glm::normalize(glm::cross(front, up));
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::processMouseScroll(float yoffset)
{
    position += front * yoffset;
}

void Camera::processMouseMovementFree(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 frontTemp;
    frontTemp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontTemp.y = sin(glm::radians(pitch));
    frontTemp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(frontTemp);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));
}
