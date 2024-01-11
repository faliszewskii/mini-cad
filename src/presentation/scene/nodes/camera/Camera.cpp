//
// Created by faliszewskii on 07.01.24.
//

#include "Camera.h"
#include "../../../../logic/algebra/AlgebraUtils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>
#include <utility>


Camera::Camera(std::string name, CameraMode cameraMode, glm::vec3 position, glm::vec3 anchor, glm::quat orientation) :
        SceneNode(std::move(name)), cameraType(cameraMode), position(position), anchor(anchor), orientation(orientation),
        movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoomSensitivity(ZOOM_SENSITIVITY) {
    // TODO Set orientation to anchor
    updateDirections();
}

void Camera::updateDirections() {
    front = AlgebraUtils::getFront(orientation);
    up = AlgebraUtils::getUp(orientation);
    right = AlgebraUtils::getRight(orientation);
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    (this->*keyboardHandlerMapping[cameraType])(direction, deltaTime);
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    (this->*mouseHandlerMapping[cameraType])(xoffset, yoffset);
}

void Camera::processMouseMovementAnchor(float xoffset, float yoffset)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    glm::quat rotationX = glm::angleAxis(xoffset, up);
    glm::quat rotationY = glm::angleAxis(yoffset, right);

    position = glm::vec3(rotationX * rotationY * glm::vec4(position - anchor, 1)) + anchor;
    orientation = glm::normalize(rotationX * rotationY * orientation);
    updateDirections();
}

void Camera::processMouseScroll(float yoffset)
{
    yoffset *= zoomSensitivity;
    position += front * yoffset;
}

void Camera::processMouseMovementFree(float xoffset, float yoffset) {
    // TODO
//    xoffset *= mouseSensitivity;
//    yoffset *= mouseSensitivity;
//
//    yaw   += xoffset;
//    pitch += yoffset;
//    if (pitch > 89.0f)
//        pitch = 89.0f;
//    if (pitch < -89.0f)
//        pitch = -89.0f;
//
//    glm::vec3 frontTemp;
//    frontTemp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//    frontTemp.y = sin(glm::radians(pitch));
//    frontTemp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//    front = glm::normalize(frontTemp);
//    // also re-calculate the Right and Up vector
//    right = glm::normalize(glm::cross(front, worldUp));
}

void Camera::processKeyboardFree(CameraMovement direction, float deltaTime) {
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

void Camera::processKeyboardAnchor(CameraMovement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        anchor += front * velocity;
    if (direction == BACKWARD)
        anchor -= front * velocity;
    if (direction == LEFT)
        anchor -= right * velocity;
    if (direction == RIGHT)
        anchor += right * velocity;

    //TODO move orientation
    updateDirections();
}

int Camera::acceptVisit(SceneNodeVisitor& visitor) {
    return visitor.visitCamera(*this);
}

int Camera::acceptLeave(SceneNodeVisitor &visitor) {
    return visitor.leaveCamera(*this);
}