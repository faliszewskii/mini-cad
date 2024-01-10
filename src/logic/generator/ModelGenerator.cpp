//
// Created by faliszewskii on 29.12.23.
//

#include "ModelGenerator.h"

#include <utility>
#include "../../presentation/scene/nodes/transformation/Transformation.h"

std::unique_ptr<SceneTreeNode> ModelGenerator::generateAxis(glm::vec3 xAxis, glm::vec3 yAxis, bool isRightHanded) {
    std::unique_ptr<SceneTreeNode> axisNode(std::make_unique<SceneTreeNode>((std::make_unique<Transformation>(Transformation("Axis")))));
    axisNode->addChild(generateLine("xAxis", glm::vec3(), glm::normalize(xAxis), glm::vec4(1,0,0,1)));
    axisNode->addChild(generateLine("yAxis", glm::vec3(), glm::normalize(yAxis), glm::vec4(0,1,0,1)));
    axisNode->addChild(generateLine("zAxis", glm::vec3(), glm::normalize(glm::cross(xAxis, yAxis))*=(isRightHanded?1:-1), glm::vec4(0,0,1,1)));
    return axisNode;
}

std::unique_ptr<Mesh> ModelGenerator::generateLine(std::string name, glm::vec3 start, glm::vec3 end, glm::vec4 color) {
    Vertex v1(start, glm::vec3(), glm::vec2()); // TODO Take care of different vertex structures.
    Vertex v2(end, glm::vec3(), glm::vec2());
    return std::make_unique<Mesh>(Mesh(std::move(name), std::vector{v1, v2}, Material(color), {}, {}, GL_LINE_STRIP));
}
