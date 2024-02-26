//
// Created by faliszewskii on 19.02.24.
//

#ifndef OPENGL_SANDBOX_SPHEREGENERATORMANUAL_H
#define OPENGL_SANDBOX_SPHEREGENERATORMANUAL_H

#include "MeshGenerator.h"
#include "MeshGeneratorHelpers.h"

class SphereGeneratorManual : public MeshGenerator {
    int meridianCount;
    int parallelCount;

public:
    explicit SphereGeneratorManual(Mesh &target) : MeshGenerator(target), meridianCount(10), parallelCount(10) {
        generate();
    }

    void generate() final {
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

                MeshGeneratorHelpers::addQuad(indices, i0, i1, i2, i3);
            }
        }

        target.update(std::move(vertices), std::move(indices));
    }

    ParameterMap getParameters() final {
        return ParameterMap {
                {"Meridian Count", meridianCount},
                {"Parallel Count", parallelCount},
        };
    }

    std::string getName() final {
        return "Sphere Generator";
    }
};

#endif //OPENGL_SANDBOX_SPHEREGENERATORMANUAL_H
