//
// Created by faliszewskii on 29.12.23.
//

#ifndef OPENGL_SANDBOX_MODELGENERATOR_H
#define OPENGL_SANDBOX_MODELGENERATOR_H


#include <glm/glm.hpp>
#include "../../presentation/scene/nodes/mesh/Mesh.h"

class ModelGenerator {
public:
    static std::vector<std::unique_ptr<SceneNode>>
    generateAxis(glm::vec3 xAxis = glm::vec3(1, 0, 0), glm::vec3 yAxis = glm::vec3(0, 1, 0), bool isRightHanded = true);

    static std::vector<std::unique_ptr<SceneNode>>
    generatePointLightRepresentation(std::reference_wrapper<std::unique_ptr<Light>> light);

    static std::vector<std::unique_ptr<SceneNode>>
    generateLine(std::string name, glm::vec3 start, glm::vec3 end, glm::vec4 color, const std::string &materialName);

    static std::unique_ptr<SceneNode> generateSphere(int meridianCount, int parallelCount);

};


#endif //OPENGL_SANDBOX_MODELGENERATOR_H
