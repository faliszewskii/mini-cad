//
// Created by faliszewskii on 30.12.23.
//

#include "AlgebraUtils.h"

// https://stackoverflow.com/questions/17918033/glm-decompose-mat4-into-translation-and-rotation
void AlgebraUtils::decomposeMtx(const glm::mat<4, 4, double> &m, glm::vec<3, double> &pos, glm::qua<double> &rot, glm::vec<3, double> &scale) {
    pos = m[3];
    for (int i = 0; i < 3; i++)
        scale[i] = glm::length(glm::vec3(m[i]));
    const glm::mat3 rotMtx(
            glm::vec<3, double>(m[0]) / scale[0],
            glm::vec<3, double>(m[1]) / scale[1],
            glm::vec<3, double>(m[2]) / scale[2]);
    rot = glm::quat_cast(rotMtx);
}
