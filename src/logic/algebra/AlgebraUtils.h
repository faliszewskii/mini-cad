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

    static void decomposeMtx(const glm::mat4& m, glm::vec3& pos, glm::quat& rot, glm::vec3& scale);
};


#endif //OPENGL_SANDBOX_ALGEBRAUTILS_H
