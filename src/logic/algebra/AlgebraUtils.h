//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_ALGEBRAUTILS_H
#define OPENGL_SANDBOX_ALGEBRAUTILS_H


#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <glm/gtx/io.hpp>

class AlgebraUtils {
    static glm::quat quaternionFromMatrix(glm::mat3 a);

public:
    static glm::vec3 getFront(glm::quat q) {
        return glm::normalize(q * glm::vec3(0, 0, -1.0f));
    }

    static glm::vec3 getUp(glm::quat q) {
        return glm::normalize(q * glm::vec3(0, 1.0f, 0));
    }

    static glm::vec3 getRight(glm::quat q) {
        return glm::normalize(q * glm::vec3(1.0f, 0, 0));
    }
//
//    static void test() {
//        glm::vec3 anchor{};
//        glm::vec3 pos(0, 1, 2);
//        std::cout<< "Pos: " << pos << std::endl;
//        glm::vec3 front = glm::normalize(anchor - pos);
//        std::cout<< "Front: " << front << std::endl;
//
//        float angle = acos(glm::dot(glm::vec3(0,0,-1), front));
//        glm::vec3 axis = glm::normalize(glm::cross(glm::vec3(0,0,-1), front));
//        glm::vec3 up(0,1,0);
//        up = glm::rotate(angle, axis);
//        std::cout<< "Up: " << up << std::endl;
//
//
//        glm::vec3 right(glm::normalize(glm::cross(front, up)));
//        std::cout<< "Right: " << right << std::endl;
//        glm::quat orientation(1,0,0,0);
//        orientation = glm::rotate(orientation, angle, axis);
//
//        glm::vec3 upTemp = glm::normalize(orientation * glm::vec3(0, 1.0f, 0));
//        glm::vec3 front1 = glm::normalize(orientation * glm::vec3(0, 0, -1.0f));
//
//        angle = acos(glm::dot(upTemp, up));
//        orientation = glm::rotate(orientation, angle, front1);
////        glm::mat3 firstMatrix(front, up, right);
//
////        glm::quat qYaw = glm::angleAxis(glm::radians(0), up);
////        glm::quat qPitch = glm::angleAxis(glm::radians(-pitch), right);
////        glm::quat qRoll = glm::angleAxis(glm::radians(roll), front);
//
////        orientation = glm::normalize(qRoll * qPitch * qYaw * orientation);
////        glm::quat orientation(quaternionFromMatrix(firstMatrix));
//
//        std::cout<< "New front: " << front1 << std::endl;
//        glm::vec3 up1 = glm::normalize(orientation * glm::vec3(0, 1.0f, 0));
//        std::cout<< "New up: " << up1 << std::endl;
//        glm::vec3 right1 = glm::normalize(orientation * glm::vec3(1.0f, 0, 0));
//        std::cout<< "New right: " << right1 << std::endl;
//
////        glm::mat4 m1 = glm::lookAt(pos, anchor,up);
////        std::cout<<m1 <<std::endl;
////        glm::quat q = glm::quat_cast(m1);
////        std::cout<<q <<std::endl;
////        glm::vec3 newFront = glm::rotate(glm::inverse(q), glm::vec3(0,0,-1));
////        glm::vec3 newUp = glm::rotate(glm::inverse(q), glm::vec3(0,1,0));
////        std::cout<< newUp <<std::endl;
//    }
};


#endif //OPENGL_SANDBOX_ALGEBRAUTILS_H
