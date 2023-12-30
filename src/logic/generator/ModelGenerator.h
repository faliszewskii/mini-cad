//
// Created by faliszewskii on 29.12.23.
//

#ifndef OPENGL_SANDBOX_MODELGENERATOR_H
#define OPENGL_SANDBOX_MODELGENERATOR_H


#include <glm/vec3.hpp>
#include "../../presentation/rendering/model/Mesh.h"
#include "../../presentation/rendering/model/ModelNode.h"

class ModelGenerator {
public:
    static ModelNode generateAxis(glm::vec3 xAxis = glm::vec3(1,0,0), glm::vec3 yAxis = glm::vec3(0,1,0), bool isRightHanded = true);
    static Mesh generateLine(glm::vec3 start, glm::vec3 end, glm::vec4 color);
};


#endif //OPENGL_SANDBOX_MODELGENERATOR_H
