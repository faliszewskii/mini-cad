//
// Created by faliszewskii on 18.02.24.
//

#ifndef OPENGL_SANDBOX_TORUS_H
#define OPENGL_SANDBOX_TORUS_H

#include "../entities/mesh/Mesh.h"
#include "GeometryHelpers.h"
#include "../vertices/Vertex.h"
#include "../entities/transformation/Transformation.h"
#include "../ids/IdCounter.h"

class Torus {
    Mesh<Vertex> mesh;

public:
    Transformation transform;
    int id;
    std::string name;
    bool selected;
    int radialResolution;
    int tubularResolution;
    float radius;
    float thickness;

    explicit Torus(glm::vec3 position = {}, int radialResolution = 50, int tubularResolution = 50, float radius = 0.75, float thickness = 0.25f)
            : radialResolution(radialResolution), tubularResolution(tubularResolution), id(IdCounter::nextId()), name("Torus (" + std::to_string(id) + ")"),
              selected(false), radius(radius), thickness(thickness), mesh({},{},GL_LINES), transform(position) {
        generate();
    }

    Torus(Torus &&t)  noexcept : radialResolution(t.radialResolution), tubularResolution(t.tubularResolution), name(t.name), id(t.id),
            selected(t.selected), radius(t.radius), thickness(t.thickness), mesh(std::move(t.mesh)), transform(t.transform)  {}

    void render(Shader &shader) {
        shader.setUniform("selected", selected);
        shader.setUniform("model", transform.getTransformation());
        mesh.render();
    }

    void generate() {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i < radialResolution; i++) {
            for (int j = 0; j < tubularResolution; j++) {
                float u = (float)j / (float)tubularResolution * M_PI * 2.0f;
                float v = (float)i / (float)radialResolution * M_PI * 2.0f;
                float x = (radius + thickness * std::cos(v)) * std::cos(u);
                float y = thickness * std::sin(v);
                float z = (radius + thickness * std::cos(v)) * std::sin(u);
                vertices.push_back(Vertex(glm::vec3{x, y, z}, getNormal(u, v)));
            }
        }

        // add quad faces
        for (int i = 0; i < radialResolution; i++) {
            auto i_next = (i + 1) % radialResolution;
            for (int j = 0; j < tubularResolution; j++) {
                auto j_next = (j + 1) % tubularResolution;
                auto i0 = i * tubularResolution + j;
                auto i1 = i * tubularResolution + j_next;
                auto i3 = i_next * tubularResolution + j;

                indices.push_back(i0);
                indices.push_back(i1);
                indices.push_back(i0);
                indices.push_back(i3);
            }
        }

        mesh.update(std::move(vertices), std::move(indices));
    }

    [[nodiscard]] glm::vec3 evaluate(float u, float v) const {
        u *= M_PI * 2.0f;
        v *= M_PI * 2.0f;
        float x = (radius + thickness * std::cos(v)) * std::cos(u);
        float y = thickness * std::sin(v);
        float z = (radius + thickness * std::cos(v)) * std::sin(u);
        return transform.getTransformation() * glm::vec4{x, y, z, 1};
    }

    [[nodiscard]] glm::vec3 getNormal(float u, float v) const {
        float xn = std::cos(v) * std::cos(u);
        float yn = std::sin(v);
        float zn = std::cos(v) * std::sin(u);
        return glm::normalize(glm::vec3(xn, yn, zn));
    }

    [[nodiscard]] glm::vec3 evaluateDU(float u, float v) const {
        u *= M_PI * 2.0f;
        v *= M_PI * 2.0f;
        float x = (radius + thickness * std::cos(v)) * -std::sin(u);
        float y = 0;
        float z = (radius + thickness * std::cos(v)) * std::cos(u);
        return glm::inverse(glm::transpose(transform.getTransformation())) * glm::vec4{x, y, z, 0};
    }

    [[nodiscard]] glm::vec3 evaluateDV(float u, float v) const {
        u *= M_PI * 2.0f;
        v *= M_PI * 2.0f;
        float x = thickness * -std::sin(v) * std::cos(u);
        float y = thickness * std::cos(v);
        float z = thickness * -std::sin(v) * std::sin(u);
        return glm::inverse(glm::transpose(transform.getTransformation())) * glm::vec4{x, y, z, 0};
    }
};

#endif //OPENGL_SANDBOX_TORUS_H
