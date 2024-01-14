//
// Created by faliszewskii on 29.12.23.
//

#include "ModelGenerator.h"

#include <utility>
#include "../../presentation/scene/nodes/transformation/Transformation.h"
#include "../io/IOUtils.h"
#include "../../presentation/scene/nodes/camera/Camera.h"

std::unique_ptr<SceneTreeNode> ModelGenerator::generateAxis(glm::vec3 xAxis, glm::vec3 yAxis, bool isRightHanded) {
    std::unique_ptr<SceneTreeNode> axisNode(std::make_unique<SceneTreeNode>((std::make_unique<Transformation>(Transformation("Axis")))));
    axisNode->addChild(generateLine("xAxis", glm::vec3(), glm::normalize(xAxis), glm::vec4(1,0,0,1), "Red"));
    axisNode->addChild(generateLine("yAxis", glm::vec3(), glm::normalize(yAxis), glm::vec4(0,1,0,1), "Green"));
    axisNode->addChild(generateLine("zAxis", glm::vec3(), glm::normalize(glm::cross(xAxis, yAxis))*=(isRightHanded?1:-1), glm::vec4(0,0,1,1), "Blue"));
    return axisNode;
}

std::unique_ptr<SceneTreeNode> ModelGenerator::generateLine(std::string name, glm::vec3 start, glm::vec3 end, glm::vec4 color, const std::string& materialName) {
    std::unique_ptr<SceneTreeNode> materialNode(std::make_unique<SceneTreeNode>((std::make_unique<Material>(materialName, color))));
    Vertex v1(start, glm::vec3(), glm::vec2()); // TODO Take care of different vertex structures.
    Vertex v2(end, glm::vec3(), glm::vec2());
    std::unique_ptr<Mesh> meshNode = std::make_unique<Mesh>(Mesh(std::move(name), std::vector{v1, v2}, {}, GL_LINE_STRIP));
    materialNode->addChild(std::move(meshNode));
    return materialNode;
}

std::unique_ptr<SceneTreeNode> ModelGenerator::generatePointLightRepresentation(std::reference_wrapper<std::unique_ptr<Light>> light) {
    auto mainNode(std::make_unique<SceneTreeNode>((std::make_unique<Transformation>(Transformation("Point light representation")))));

    auto shader = std::make_unique<Shader>("albedo", IOUtils::getResource("shaders/basic/albedo.vert"), IOUtils::getResource("shaders/basic/albedo.frag"));
    auto shaderNode = std::make_unique<SceneTreeNode>(std::move(shader));

    auto sourceTransformation = std::make_unique<Transformation>(Transformation("Light source"));
    sourceTransformation->setScale(glm::vec3(0.02));
    auto sourceNode(std::make_unique<SceneTreeNode>(std::move(sourceTransformation)));
    auto sourceMaterialNode(std::make_unique<SceneTreeNode>((std::make_unique<Material>("White", glm::vec4(1,1,0,1))))); // TODO Bind material color to light color
    auto sourceMeshNode = generateSphere(10, 10);
    sourceMaterialNode->addChild(std::move(sourceMeshNode));
    sourceNode->addChild(std::move(sourceMaterialNode));

    auto bulbTransformation = std::make_unique<Transformation>(Transformation("Bulb"));
    // TODO Bind transformation position to light position.
    bulbTransformation->setScale(glm::vec3(0.1));
    auto bulbNode(std::make_unique<SceneTreeNode>(std::move(bulbTransformation)));
    auto bulbMaterialNode(std::make_unique<SceneTreeNode>((std::make_unique<Material>("Bulb material", glm::vec4(1,1,1,0.2f)))));
    auto bulbMeshNode = generateSphere(10, 10);
    bulbMaterialNode->addChild(std::move(bulbMeshNode));
    bulbNode->addChild(std::move(bulbMaterialNode));

    shaderNode->addChild(std::move(sourceNode));
    mainNode->addChild(std::move(shaderNode));
    mainNode->addChild(std::move(bulbNode));
    return mainNode;
}


std::unique_ptr<SceneTreeNode> ModelGenerator::generateSphere(int meridianCount, int parallelCount) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // add top vertex
    vertices.push_back(Vertex(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec2())); // TODO Textured sphere?

    // generate vertices per stack / slice
    for (int i = 0; i < parallelCount - 1; i++)
    {
        auto phi = M_PI * double(i + 1) / double(parallelCount);
        for (int j = 0; j < meridianCount; j++)
        {
            auto theta = 2.0 * M_PI * double(j) / double(meridianCount);
            auto x = std::sin(phi) * std::cos(theta);
            auto y = std::cos(phi);
            auto z = std::sin(phi) * std::sin(theta);
            auto pos = glm::vec3(x, y, z);
            vertices.push_back(Vertex(pos, glm::normalize(pos), glm::vec2()));
        }
    }

    // add bottom vertex
    vertices.push_back(Vertex(glm::vec3(0, -1, 0), glm::vec3(0, -1, 0), glm::vec2()));

    for (int i = 0; i < meridianCount; ++i)
    {
        auto i0 = i + 1;
        auto i1 = (i + 1) % meridianCount + 1;
        indices.push_back(0);
        indices.push_back(i0);
        indices.push_back(i1);
        i0 = i + meridianCount * (parallelCount - 2) + 1;
        i1 = (i + 1) % meridianCount + meridianCount * (parallelCount - 2) + 1;
        indices.push_back(vertices.size()-1);
        indices.push_back(i0);
        indices.push_back(i1);
    }

    for (int j = 0; j < parallelCount - 2; j++)
    {
        auto j0 = j * meridianCount + 1;
        auto j1 = (j + 1) * meridianCount + 1;
        for (int i = 0; i < meridianCount; i++)
        {
            auto i0 = j0 + i;
            auto i1 = j0 + (i + 1) % meridianCount;
            auto i2 = j1 + (i + 1) % meridianCount;
            auto i3 = j1 + i;

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i3);
            indices.push_back(i0);
            indices.push_back(i2);
        }
    }

    return std::make_unique<SceneTreeNode>(std::make_unique<Mesh>(Mesh("UV Sphere mesh", vertices, indices, GL_TRIANGLES)));
}
