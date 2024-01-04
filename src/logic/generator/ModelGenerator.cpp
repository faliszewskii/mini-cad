//
// Created by faliszewskii on 29.12.23.
//

#include "ModelGenerator.h"

ModelNode ModelGenerator::generateAxis(glm::vec3 xAxis, glm::vec3 yAxis, bool isRightHanded) {
    Mesh xAxisModel = generateLine(glm::vec3(), glm::normalize(xAxis), glm::vec4(1,0,0,1));
    Mesh yAxisModel = generateLine(glm::vec3(), glm::normalize(yAxis), glm::vec4(0,1,0,1));
    Mesh zAxisModel = generateLine(glm::vec3(), glm::normalize(glm::cross(xAxis, yAxis))*=(isRightHanded?1:-1), glm::vec4(0,0,1,1));
    return ModelNode("Axis", std::vector{xAxisModel, yAxisModel, zAxisModel}, std::vector<ModelNode>(), glm::mat4(1.0f));
}

Mesh ModelGenerator::generateLine(glm::vec3 start, glm::vec3 end, glm::vec4 color) {
    Vertex v1(start, glm::vec3(), glm::vec2()); // TODO Take care of different vertex structures.
    Vertex v2(end, glm::vec3(), glm::vec2());
    return Mesh(std::vector{v1, v2}, Material(color), {}, {}, GL_LINE_STRIP);
}
