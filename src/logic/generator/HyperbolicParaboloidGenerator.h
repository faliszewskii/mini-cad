//
// Created by faliszewskii on 18.02.24.
//

#ifndef OPENGL_SANDBOX_HYPERBOLICPARABOLOIDGENERATOR_H
#define OPENGL_SANDBOX_HYPERBOLICPARABOLOIDGENERATOR_H

#include "ParametrisedMeshGenerator.h"

class HyperbolicParaboloidGenerator : public ParametrisedMeshGenerator {
    float a, b, c;
    float x(float u, float) override { return a * u; };
    float y(float, float v) override { return b * v; };
    float z(float u, float v) override { return c * u * v; };
    float xdu(float u, float) override { return a; };
    float ydu(float, float v) override { return b * v; };
    float zdu(float u, float v) override { return c * v; };
    float xdv(float u, float) override { return a * u; };
    float ydv(float, float v) override { return b; };
    float zdv(float u, float v) override { return c * u; };

public:
    explicit HyperbolicParaboloidGenerator(Mesh& mesh, float a = 1, float b = 1, float c = 1):
            ParametrisedMeshGenerator(mesh, 10, 10, 0, 1, 0, 1), a(a), b(b), c(c) {
        ParametrisedMeshGenerator::generate();
    }

    ParameterMap getParameters() override {
        ParameterMap map {
                {"a", a},
                {"b", b},
                {"c", c},
        };
        auto parentMap = ParametrisedMeshGenerator::getParameters();
        map.insert(map.end(),std::make_move_iterator(parentMap.begin()), std::make_move_iterator(parentMap.end()));
        return map;
    }

    std::string getName() final {
        return "Hyperbolic Paraboloid Generator";
    };
};
#endif //OPENGL_SANDBOX_HYPERBOLICPARABOLOIDGENERATOR_H
