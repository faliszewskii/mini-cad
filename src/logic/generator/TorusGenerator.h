//
// Created by faliszewskii on 18.02.24.
//

#ifndef OPENGL_SANDBOX_TORUSGENERATOR_H
#define OPENGL_SANDBOX_TORUSGENERATOR_H

#include "MeshGenerator.h"
#include "../entities/mesh/Mesh.h"
#include "MeshGeneratorHelpers.h"

class TorusGenerator : public MeshGenerator {
    int radialResolution;
    int tubularResolution;
    float radius;
    float thickness;
public:
    explicit TorusGenerator(int radialResolution = 50, int tubularResolution = 50, float radius = 1, float thickness = 0.25f)
            : radialResolution(radialResolution), tubularResolution(tubularResolution),
              radius(radius), thickness(thickness) {
        generate();
    }

    void generate() final {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i < radialResolution; i++) {
            for (int j = 0; j < tubularResolution; j++) {
                float u = (float)j / (float)tubularResolution * M_PI * 2.0f;
                float v = (float)i / (float)radialResolution * M_PI * 2.0f;
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
        for (int i = 0; i < radialResolution; i++) {
            auto i_next = (i + 1) % radialResolution;
            for (int j = 0; j < tubularResolution; j++) {
                auto j_next = (j + 1) % tubularResolution;
                auto i0 = i * tubularResolution + j;
                auto i1 = i * tubularResolution + j_next;
                auto i2 = i_next * tubularResolution + j_next;
                auto i3 = i_next * tubularResolution + j;

                MeshGeneratorHelpers::addQuad(indices, i0, i1, i2, i3);
            }
        }

        target->update(std::move(vertices), std::move(indices));
    }

    ParameterMap getParameters() final {
        return ParameterMap {
                {"Radial Resolution", radialResolution},
                {"Tubular Resolution", tubularResolution},
                {"Radius", radius},
                {"Thickness", thickness},
        };
    }

    std::string getName() final {
        return "Torus Generator";
    }
};

#endif //OPENGL_SANDBOX_TORUSGENERATOR_H
