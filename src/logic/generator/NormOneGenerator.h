//
// Created by faliszewskii on 19.02.24.
//

#ifndef OPENGL_SANDBOX_NORMONEGENERATOR_H
#define OPENGL_SANDBOX_NORMONEGENERATOR_H

#include <cmath>

#include "ParametrisedMeshGenerator.h"

class NormOneGenerator : public ParametrisedMeshGenerator {
    float e;

    static constexpr auto xp = [](float u, float v){ return float(std::sin(v) * std::cos(u));};
    static constexpr auto yp = [](float u, float v){ return float(std::sin(v) * std::sin(u));};
    static constexpr auto zp = [](float u, float v){ return float(std::cos(v));};
    static constexpr auto exponent = [](float ee){ return 1 + std::tan(ee); };

    float x(float u, float v) override { return std::copysign(1.0f, xp(u, v)) * std::pow(std::abs(xp(u, v)), exponent(e)); };
    float y(float u, float v) override { return std::copysign(1.0f, yp(u, v)) * std::pow(std::abs(yp(u, v)), exponent(e)); };
    float z(float u, float v) override { return std::copysign(1.0f, zp(u, v)) * std::pow(std::abs(zp(u, v)), exponent(e)); };
    float xdu(float u, float) override { return 0; };
    float ydu(float u, float v) override { return 0; };
    float zdu(float u, float v) override { return 0; };
    float xdv(float u, float v) override { return 0; };
    float ydv(float u, float v) override { return 0; };
    float zdv(float u, float v) override { return 0; };

public:
    explicit NormOneGenerator(float e = 0) :
            ParametrisedMeshGenerator(10, 10, 0, 2 * M_PI, 0, M_PI), e(e) {
        ParametrisedMeshGenerator::generate();
    }

    ParameterMap getParameters() override {
        ParameterMap map {
                {"e", e},
        };
        auto parentMap = ParametrisedMeshGenerator::getParameters();
        map.insert(map.end(),std::make_move_iterator(parentMap.begin()), std::make_move_iterator(parentMap.end()));
        return map;
    }

    std::string getName() final {
        return "Norm One Family Generator";
    };
};

#endif //OPENGL_SANDBOX_NORMONEGENERATOR_H
