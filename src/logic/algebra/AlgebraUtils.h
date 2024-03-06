//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_ALGEBRAUTILS_H
#define OPENGL_SANDBOX_ALGEBRAUTILS_H


#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <glm/gtx/io.hpp>

class AlgebraUtils {
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

    static void decomposeMtx(const glm::mat<4, 4, double> &m, glm::vec<3, double> &pos, glm::qua<double> &rot, glm::vec<3, double> &scale);
};


#endif //OPENGL_SANDBOX_ALGEBRAUTILS_H
