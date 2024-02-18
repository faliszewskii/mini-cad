//
// Created by faliszewskii on 29.12.23.
//
// Sphere and torus based off https://danielsieger.com/blog/2021/05/03/generating-primitive-shapes.html
// by Daniel Sieger.

#include "ModelGenerator.h"

#include <utility>

std::unique_ptr<Scene> ModelGenerator::generateAxis(glm::vec3 xAxis, glm::vec3 yAxis, bool isRightHanded) {
    auto result = std::make_unique<Scene>();

    auto &axisTransformation = result->addSceneNode(Transformation("Axis"));
    auto &axisNode = result->addStep(PushTransformation(axisTransformation));

    auto red = generateLine("xAxis", glm::vec3(), glm::normalize(xAxis), glm::vec4(1, 0, 0, 1), "Red");
    result->merge(std::move(red), axisNode);
    auto green = generateLine("yAxis", glm::vec3(), glm::normalize(yAxis), glm::vec4(0, 1, 0, 1), "Green");
    result->merge(std::move(green), axisNode);
    auto blue = generateLine("zAxis", glm::vec3(), glm::normalize(glm::cross(xAxis, yAxis)) *= (isRightHanded ? 1 : -1),
                             glm::vec4(0, 0, 1, 1), "Blue");
    result->merge(std::move(blue), axisNode);

    return result;
}

std::unique_ptr<Scene> ModelGenerator::generateLine(std::string name, glm::vec3 start, glm::vec3 end, glm::vec4 color, const std::string &materialName) {
    auto result = std::make_unique<Scene>();

    auto& material = result->addSceneNode(Material(materialName, color));
    auto& materialNode = result->addStep(AddMaterial(material));

    Vertex v1(start, glm::vec3(), glm::vec2()); // TODO Take care of different vertex structures.
    Vertex v2(end, glm::vec3(), glm::vec2());

    auto &mesh = result->addSceneNode(Mesh(std::move(name), std::vector{v1, v2}, {}, {}, GL_LINE_STRIP));
    result->addStep(DrawMesh(mesh), materialNode);

    return result;
}

Mesh ModelGenerator::generateSphereMesh(int meridianCount, int parallelCount) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // add top vertex
    vertices.push_back(Vertex(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec2())); // TODO Textured sphere?

    // generate vertices per stack / slice
    for (int i = 0; i < parallelCount - 1; i++) {
        auto phi = M_PI * double(i + 1) / double(parallelCount);
        for (int j = 0; j < meridianCount; j++) {
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

    for (int i = 0; i < meridianCount; ++i) {
        auto i0 = i + 1;
        auto i1 = (i + 1) % meridianCount + 1;
        indices.push_back(0);
        indices.push_back(i0);
        indices.push_back(i1);
        i0 = i + meridianCount * (parallelCount - 2) + 1;
        i1 = (i + 1) % meridianCount + meridianCount * (parallelCount - 2) + 1;
        indices.push_back(vertices.size() - 1);
        indices.push_back(i0);
        indices.push_back(i1);
    }

    for (int j = 0; j < parallelCount - 2; j++) {
        auto j0 = j * meridianCount + 1;
        auto j1 = (j + 1) * meridianCount + 1;
        for (int i = 0; i < meridianCount; i++) {
            auto i0 = j0 + i;
            auto i1 = j0 + (i + 1) % meridianCount;
            auto i2 = j1 + (i + 1) % meridianCount;
            auto i3 = j1 + i;

            addQuad(indices, i0, i1, i2, i3);
        }
    }

    return {"UV Sphere mesh", vertices, indices, {}, GL_TRIANGLES};
}

std::unique_ptr<Scene> ModelGenerator::generateSphere(int meridianCount, int parallelCount) {
    return generateSolid(generateSphereMesh(meridianCount, parallelCount), "UV Sphere");
}

Mesh ModelGenerator::generateTorusMesh(int radial_resolution, int tubular_resolution, float thickness, float radius) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < radial_resolution; i++) {
        for (int j = 0; j < tubular_resolution; j++) {
            float u = (float)j / (float)tubular_resolution * M_PI * 2.0f;
            float v = (float)i / (float)radial_resolution * M_PI * 2.0f;
            float x = (radius + thickness * std::cos(v)) * std::cos(u);
            float y = (radius + thickness * std::cos(v)) * std::sin(u);
            float z = thickness * std::sin(v);
            float xn = std::cos(v) * std::cos(u);
            float yn = std::cos(v) * std::sin(u);
            float zn = std::sin(v);
            vertices.push_back(Vertex(glm::vec3(x, y, z), glm::normalize(glm::vec3(xn, yn, zn))));
        }
    }

    // add quad faces
    for (int i = 0; i < radial_resolution; i++) {
        auto i_next = (i + 1) % radial_resolution;
        for (int j = 0; j < tubular_resolution; j++) {
            auto j_next = (j + 1) % tubular_resolution;
            auto i0 = i * tubular_resolution + j;
            auto i1 = i * tubular_resolution + j_next;
            auto i2 = i_next * tubular_resolution + j_next;
            auto i3 = i_next * tubular_resolution + j;

            addQuad(indices, i0, i1, i2, i3);
        }
    }
    return {"Torus mesh", vertices, indices, {}, GL_TRIANGLES};
}

void ModelGenerator::addQuad(std::vector<unsigned int> &indices, int i0, int i1, int i2, int i3) {
    indices.push_back(i0);
    indices.push_back(i1);
    indices.push_back(i2);

    indices.push_back(i3);
    indices.push_back(i0);
    indices.push_back(i2);
}

std::unique_ptr<Scene> ModelGenerator::generateTorus(int radial_resolution, int tubular_resolution, float thickness, float radius) {
    return generateSolid(generateTorusMesh(radial_resolution, tubular_resolution, thickness, radius), "Torus");
}

std::unique_ptr<Scene> ModelGenerator::generateSolid(Mesh &&mesh, const std::string& name) {
    auto result = std::make_unique<Scene>();
    auto &transformationNode = result->addStep(PushTransformation(result->addSceneNode(Transformation(name))));
    auto &materialNode = result->addStep(AddMaterial(result->addSceneNode(Material("White"))), transformationNode);
    result->addStep(DrawMesh(result->addSceneNode(std::move(mesh))), materialNode);
    return result;
}

std::unique_ptr<Mesh> ModelGenerator::generatePlaneMesh(glm::vec3 normal) {
    // TODO create plane based on normal
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    if(normal != glm::vec3(0,1,0)) throw "not implemented";

    vertices.push_back(Vertex(glm::vec3(1,0,-1), glm::vec3(0,1,0)));
    vertices.push_back(Vertex(glm::vec3(1,0,1), glm::vec3(0,1,0)));
    vertices.push_back(Vertex(glm::vec3(-1,0,1), glm::vec3(0,1,0)));
    vertices.push_back(Vertex(glm::vec3(-1,0,-1), glm::vec3(0,1,0)));

    addQuad(indices,0, 1, 2, 3);

    return std::make_unique<Mesh>(Mesh("Plane mesh", vertices, indices, {}, GL_TRIANGLES));
}

/// Generates mesh based on 3D parametrised equations. It draws using u, v from min inclusive to max exclusive if repeating, max inclusive if not.
Mesh ModelGenerator::ParametrisedModelGenerator::generateParametrisedMesh(const std::string& name, int uCount, int vCount) {
    // TODO Add rules for connecting edges (if closed) and in what direction to connect them.
    // TODO Add indices.
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float uStep = 1.f / (float)(uCount-1) * (uMax - uMin);
    float vStep = 1.f / (float)(vCount-1) * (vMax - vMin);
    for (int i = 0; i < uCount; i++) { // +1 or connection rules in indices
        float u = (float)i * uStep + uMin;
        for (int j = 0; j < vCount; j++) {
            float v = (float)j * vStep + vMin;
            auto pos = glm::vec3(x(u, v), y(u, v), z(u, v));
            auto U = glm::vec3(xdu(u, v), ydu(u, v), zdu(u, v));
            auto V = glm::vec3(xdv(u, v), ydv(u, v), zdv(u, v));
            vertices.emplace_back(pos, glm::normalize(glm::cross(U,V)));
        }
    }
    for (int i = 0; i < uCount-1; i++) {
        auto i_next = (i + 1);
        for (int j = 0; j < vCount-1; j++) {
            auto j_next = (j + 1);
            auto i0 = i * vCount + j;
            auto i1 = i * vCount + j_next;
            auto i2 = i_next * vCount + j_next;
            auto i3 = i_next * vCount + j;

            addQuad(indices, i0, i1, i2, i3);
        }
    }
    return {name, vertices, indices, {}, GL_TRIANGLES};
}
