//
// Created by faliszewskii on 26.02.24.
//

#ifndef OPENGL_SANDBOX_CIRCLEGENERATOR_H
#define OPENGL_SANDBOX_CIRCLEGENERATOR_H


#include <cmath>
#include <glm/gtc/type_ptr.hpp>

#include "ParametrisedMeshGenerator.h"

class CircleGenerator : public ParametrisedMeshGenerator {

    constexpr static const glm::vec<3, double> a{0,1,0};
    constexpr static const glm::vec<3, double> b0{1,0,0};
    constexpr static const glm::vec<3, double> b1{0,0,1};
    float R;
    glm::vec<3, float> p0;

    float x(float u, float v) override { return  p0.x + std::cos(u) * R * v * b0.x + sin(u) * R * v * b1.x; };
    float y(float u, float v) override { return p0.y + std::cos(u) * R * v * b0.y + sin(u) * R * v * b1.y; };
    float z(float u, float v) override { return p0.z +std::cos(u) * R * v * b0.z + sin(u) * R * v * b1.z; };
    float xdu(float u, float) override { return 0; };
    float ydu(float u, float v) override { return 0; };
    float zdu(float u, float v) override { return 0; };
    float xdv(float u, float v) override { return 0; };
    float ydv(float u, float v) override { return 0; };
    float zdv(float u, float v) override { return 0; };

public:
    explicit CircleGenerator(glm::vec<3,double> p0 = {0,0,0}, float R = 1) :
            ParametrisedMeshGenerator(50, 50, 0, 2 * M_PI, 0, 1),
            R(R), p0(p0) {
        ParametrisedMeshGenerator::generate();
    }

    ParameterMap getParameters() override {
        ParameterMap map {
                {"R", R},
                {"p0x", *(glm::value_ptr(p0) + 0)},
                {"p0y", *(glm::value_ptr(p0) + 1)},
                {"p0z", *(glm::value_ptr(p0) + 2)}
        };
        auto parentMap = ParametrisedMeshGenerator::getParameters();
        map.insert(map.end(),std::make_move_iterator(parentMap.begin()), std::make_move_iterator(parentMap.end()));
        return map;
    }

    std::string getName() final {
        return "Circle Generator";
    };
};

#endif //OPENGL_SANDBOX_CIRCLEGENERATOR_H
