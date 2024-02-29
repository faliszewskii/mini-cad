//
// Created by faliszewskii on 26.02.24.
//

#ifndef OPENGL_SANDBOX_TORUSGENERATOR_H
#define OPENGL_SANDBOX_TORUSGENERATOR_H

#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ParametrisedMeshGenerator.h"

class TorusGenerator : public ParametrisedMeshGenerator {

    constexpr static const glm::vec<3, double> a{0,1,0};
    constexpr static const glm::vec<3, double> b0{1,0,0};
    constexpr static const glm::vec<3, double> b1{0,0,1};
    float radius, thickness;
    glm::vec<3, float> p0;

    float x(float u, float v) override { return radius * sin(v) * b1.x + radius * cos(v) * b0.x + thickness * cos(u)*cos(v) * b0.x + thickness * cos(u)*sin(v) * b1.x + thickness * sin(u) * a.x; };
    float y(float u, float v) override { return radius * sin(v) * b1.y + radius * cos(v) * b0.y + thickness * cos(u)*cos(v) * b0.y + thickness * cos(u)*sin(v) * b1.y + thickness * sin(u) * a.y;  };
    float z(float u, float v) override { return radius * sin(v) * b1.z + radius * cos(v) * b0.z + thickness * cos(u)*cos(v) * b0.z + thickness * cos(u)*sin(v) * b1.z + thickness * sin(u) * a.z;  };
    float xdu(float u, float) override { return 0; };
    float ydu(float u, float v) override { return 0; };
    float zdu(float u, float v) override { return 0; };
    float xdv(float u, float v) override { return 0; };
    float ydv(float u, float v) override { return 0; };
    float zdv(float u, float v) override { return 0; };

public:
    explicit TorusGenerator(glm::vec<3,double> p0 = {0,0,0}, float radius = 1, float thickness = 0.25) :
            ParametrisedMeshGenerator(50, 50, 0, 2 * M_PI, 0, 2 * M_PI),
            radius(radius), thickness(thickness), p0(p0) {
        ParametrisedMeshGenerator::generate();
    }

    ParameterMap getParameters() override {
        ParameterMap map {
                {"radius", radius},
                {"thickness", radius},
                {"p0x", *(glm::value_ptr(p0) + 0)},
                {"p0y", *(glm::value_ptr(p0) + 1)},
                {"p0z", *(glm::value_ptr(p0) + 2)}
        };
        auto parentMap = ParametrisedMeshGenerator::getParameters();
        map.insert(map.end(),std::make_move_iterator(parentMap.begin()), std::make_move_iterator(parentMap.end()));
        return map;
    }

    std::string getName() final {
        return "Torus Generator";
    };
};

#endif //OPENGL_SANDBOX_TORUSGENERATOR_H
