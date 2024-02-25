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

//    float x(float u, float v) override { return std::copysign(1.0f, xp(u, v)) * std::pow(std::abs(xp(u, v)), exponent(e)); };
//    float y(float u, float v) override { return std::copysign(1.0f, yp(u, v)) * std::pow(std::abs(yp(u, v)), exponent(e)); };
//    float z(float u, float v) override { return std::copysign(1.0f, zp(u, v)) * std::pow(std::abs(zp(u, v)), exponent(e)); };
//    float xdu(float u, float) override { return 0; };
//    float ydu(float u, float v) override { return 0; };
//    float zdu(float u, float v) override { return 0; };
//    float xdv(float u, float v) override { return 0; };
//    float ydv(float u, float v) override { return 0; };
//    float zdv(float u, float v) override { return 0; };

    constexpr static const glm::vec3 a{0,1,0};
    constexpr static const glm::vec3 b0{1,0,0};
    constexpr static const glm::vec3 b1{0,0,1};
    constexpr const static float r = 1;
//
//    Walec
//    float x(float u, float v) override { return u*a.x + r * cos(v) * glm::normalize(b0).x + r * sin(v) * glm::normalize(b1).x; };
//    float y(float u, float v) override { return u*a.y + r * cos(v) * glm::normalize(b0).y + r * sin(v) * glm::normalize(b1).y;  };
//    float z(float u, float v) override { return u*a.z + r * cos(v) * glm::normalize(b0).z + r * sin(v) * glm::normalize(b1).z;  };
//
//    Kula
//    float x(float u, float v) override { return r*sin(u)*a.x + r * cos(u) * cos(v) * glm::normalize(b0).x + r * cos(u) * sin(v) * glm::normalize(b1).x; };
//    float y(float u, float v) override { return r*sin(u)*a.y + r * cos(u) * cos(v) * glm::normalize(b0).y + r * cos(u) * sin(v) * glm::normalize(b1).y; };
//    float z(float u, float v) override { return r*sin(u)*a.z + r * cos(u) * cos(v) * glm::normalize(b0).z + r * cos(u) * sin(v) * glm::normalize(b1).z; };
//  Torus
    constexpr const static float R = 3;
    float x(float u, float v) override { return R * sin(v) * b1.x + R * cos(v) * b0.x + r * cos(u)*cos(v) * b0.x + r * cos(u)*sin(v) * b1.x + r * sin(u) * a.x; };
    float y(float u, float v) override { return R * sin(v) * b1.y + R * cos(v) * b0.y + r * cos(u)*cos(v) * b0.y + r * cos(u)*sin(v) * b1.y + r * sin(u) * a.y;  };
    float z(float u, float v) override { return R * sin(v) * b1.z + R * cos(v) * b0.z + r * cos(u)*cos(v) * b0.z + r * cos(u)*sin(v) * b1.z + r * sin(u) * a.z;  };

    float xdu(float u, float) override { return 0; };
    float ydu(float u, float v) override { return 0; };
    float zdu(float u, float v) override { return 0; };
    float xdv(float u, float v) override { return 0; };
    float ydv(float u, float v) override { return 0; };
    float zdv(float u, float v) override { return 0; };

public:
    explicit NormOneGenerator(float e = 0) :
//            ParametrisedMeshGenerator(mesh, 10, 10, 0, 2 * M_PI, 0, M_PI), e(e) {
//            ParametrisedMeshGenerator(mesh, 100, 100, 0, 2 * M_PI, 0, M_PI), e(e) {
        ParametrisedMeshGenerator(100, 100, 0, 2 * M_PI, 0, 2*M_PI), e(e) {
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
