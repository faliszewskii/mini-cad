//
// Created by faliszewskii on 18.02.24.
//

#ifndef OPENGL_SANDBOX_TORUS_H
#define OPENGL_SANDBOX_TORUS_H
#include <glm/glm.hpp>
#include "../entities/mesh/Mesh.h"
#include "GeometryHelpers.h"
#include "../vertices/Vertex.h"
#include "../entities/transformation/Transformation.h"
#include "../ids/IdCounter.h"
#include "../texture/Texture.h"

class Torus {
    Mesh<Vertex> mesh;
    Texture mask;
    std::array<std::array<glm::vec3, 256>, 256> maskData;

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
              selected(false), radius(radius), thickness(thickness), mesh({},{},GL_LINES), transform(position),
            mask(256, 256, 4, GL_SRGB_ALPHA, GL_RGBA, GL_FLOAT, GL_TEXTURE_2D) {
        generate();
        for(auto &row : maskData)
            for(auto &el : row)
                el = glm::vec3(0);
    }

    Torus(Torus &&t)  noexcept : radialResolution(t.radialResolution), tubularResolution(t.tubularResolution), name(t.name), id(t.id),
            selected(t.selected), radius(t.radius), thickness(t.thickness), mesh(std::move(t.mesh)), transform(t.transform),
            mask(256, 256, 1, GL_SRGB_ALPHA, GL_RGBA, GL_FLOAT, GL_TEXTURE_2D) {}

    void render(Shader &shader) {
        shader.setUniform("selected", selected);
        shader.setUniform("model", transform.getTransformation());
        mask.bind(0);
        shader.setUniform("mask", 0);
        shader.setUniform("usingMask", true);
        mesh.render();
        shader.setUniform("usingMask", false);
    }

    void generate() {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i < radialResolution+1; i++) {
            for (int j = 0; j < tubularResolution+1; j++) {
                float u = (float)j / (float)tubularResolution * std::numbers::pi * 2.0f;
                float v = (float)i / (float)radialResolution * std::numbers::pi * 2.0f;
                float x = (radius + thickness * std::cos(v)) * std::cos(u);
                float y = thickness * std::sin(v);
                float z = (radius + thickness * std::cos(v)) * std::sin(u);
                vertices.push_back(Vertex(glm::vec3{x, y, z}, getNormal(u, v), {(float)i / (float)(radialResolution), (float)j / (float)tubularResolution}));
            }
        }

        // add quad faces
        for (int i = 0; i < radialResolution; i++) {
            auto i_next = (i + 1)/* % (radialResolution+1)*/;
            for (int j = 0; j < tubularResolution; j++) {
                auto j_next = (j + 1)/* % tubularResolution*/;
                auto i0 = i * (tubularResolution+1) + j;
                auto i1 = i * (tubularResolution+1) + j_next;
                auto i3 = i_next * (tubularResolution+1) + j;

                indices.push_back(i0);
                indices.push_back(i1);
                indices.push_back(i0);
                indices.push_back(i3);
            }
        }

        mesh.update(std::move(vertices), std::move(indices));
    }

    static float rangeU() { return 2 * std::numbers::pi; }
    static float rangeV() { return 2 * std::numbers::pi; }
    static bool wrapU() { return true; }
    static bool wrapV() { return true; }

    [[nodiscard]] glm::vec3 evaluate(float u, float v) const {
        // u *= std::numbers::pi * 2.0f;
        // v *= std::numbers::pi * 2.0f;
        float x = (radius + thickness * std::cos(v)) * std::cos(u);
        float y = thickness * std::sin(v);
        float z = (radius + thickness * std::cos(v)) * std::sin(u);
        return transform.getTransformation() * glm::vec4{x, y, z, 1};
    }

    [[nodiscard]] glm::vec3 evaluateDU(float u, float v) const {
        // u *= std::numbers::pi * 2.0f;
        // v *= std::numbers::pi * 2.0f;
        float x = (radius + thickness * std::cos(v)) * -std::sin(u);
        float y = 0;
        float z = (radius + thickness * std::cos(v)) * std::cos(u);
        return glm::inverse(glm::transpose(transform.getTransformation())) * glm::vec4{x, y, z, 0};
    }

    [[nodiscard]] glm::vec3 evaluateDV(float u, float v) const {
        // u *= std::numbers::pi * 2.0f;
        // v *= std::numbers::pi * 2.0f;
        float x = thickness * -std::sin(v) * std::cos(u);
        float y = thickness * std::cos(v);
        float z = thickness * -std::sin(v) * std::sin(u);
        return glm::inverse(glm::transpose(transform.getTransformation())) *  glm::vec4{x, y, z, 0};
    }

    [[nodiscard]] glm::vec3 getNormal(float u, float v) const {
        float xn = std::cos(v) * std::cos(u);
        float yn = std::sin(v);
        float zn = std::cos(v) * std::sin(u);
        return glm::normalize(glm::vec3(xn, yn, zn));
    }

    [[nodiscard]] std::pair<std::reference_wrapper<Texture>, std::reference_wrapper<std::array<std::array<glm::vec3, 256>, 256>>> getMask() {
        return std::make_pair(std::ref(mask), std::ref(maskData));
    }

    void setMask(std::array<std::array<glm::vec3, 256>, 256> &newMask) {
        maskData = newMask;
        std::vector<float> flattenedArray;
        flattenedArray.reserve(256 * 256 * 4);
        for (const auto& row : maskData) {
            for (const auto& vec : row) {
                flattenedArray.push_back(vec.x);
                flattenedArray.push_back(vec.y);
                flattenedArray.push_back(vec.z);
                flattenedArray.push_back(1);
            }
        }
        mask.update2D(flattenedArray.data());
    }
};

#endif //OPENGL_SANDBOX_TORUS_H
