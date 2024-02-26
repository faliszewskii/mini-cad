//
// Created by faliszewskii on 26.02.24.
//

#ifndef OPENGL_SANDBOX_SPHEREGENERATOR_H
#define OPENGL_SANDBOX_SPHEREGENERATOR_H

#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ParametrisedMeshGenerator.h"

class SphereGenerator : public ParametrisedMeshGenerator {

    constexpr static const glm::vec<3, double> a{0,1,0};
    constexpr static const glm::vec<3, double> b0{1,0,0};
    constexpr static const glm::vec<3, double> b1{0,0,1};
    float R;
    glm::vec<3, float> p0;

    float x(float u, float v) override { return R*sin(u)*a.x + R * cos(u) * cos(v) * glm::normalize(b0).x + R * cos(u) * sin(v) * glm::normalize(b1).x; };
    float y(float u, float v) override { return R*sin(u)*a.y + R * cos(u) * cos(v) * glm::normalize(b0).y + R * cos(u) * sin(v) * glm::normalize(b1).y; };
    float z(float u, float v) override { return R*sin(u)*a.z + R * cos(u) * cos(v) * glm::normalize(b0).z + R * cos(u) * sin(v) * glm::normalize(b1).z; };
    float xdu(float u, float) override { return 0; };
    float ydu(float u, float v) override { return 0; };
    float zdu(float u, float v) override { return 0; };
    float xdv(float u, float v) override { return 0; };
    float ydv(float u, float v) override { return 0; };
    float zdv(float u, float v) override { return 0; };

public:
    explicit SphereGenerator(glm::vec<3,double> p0 = {0,0,0}, float R = 1) :
            ParametrisedMeshGenerator(50, 50, 0, 2 * M_PI, 0, M_PI),
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
        return "Sphere Generator";
    };
};

#endif //OPENGL_SANDBOX_SPHEREGENERATOR_H
