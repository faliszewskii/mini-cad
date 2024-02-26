//
// Created by faliszewskii on 26.02.24.
//

#ifndef OPENGL_SANDBOX_CYLINDERGENERATOR_H
#define OPENGL_SANDBOX_CYLINDERGENERATOR_H

#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ParametrisedMeshGenerator.h"

class CylinderGenerator : public ParametrisedMeshGenerator {

    constexpr static const glm::vec<3, double> a{0,1,0};
    constexpr static const glm::vec<3, double> b0{1,0,0};
    constexpr static const glm::vec<3, double> b1{0,0,1};
    float R, H;
    glm::vec<3, float> p0;

    float x(float u, float v) override { return H*v*a.x + R * cos(u) * glm::normalize(b0).x + R * sin(u) * glm::normalize(b1).x; };
    float y(float u, float v) override { return H*v*a.y + R * cos(u) * glm::normalize(b0).y + R * sin(u) * glm::normalize(b1).y;  };
    float z(float u, float v) override { return H*v*a.z + R * cos(u) * glm::normalize(b0).z + R * sin(u) * glm::normalize(b1).z;  };
    float xdu(float u, float) override { return 0; };
    float ydu(float u, float v) override { return 0; };
    float zdu(float u, float v) override { return 0; };
    float xdv(float u, float v) override { return 0; };
    float ydv(float u, float v) override { return 0; };
    float zdv(float u, float v) override { return 0; };

public:
    explicit CylinderGenerator(glm::vec<3,double> p0 = {0,0,0}, float R = 1, float H = 1) :
            ParametrisedMeshGenerator(50, 50, 0, 2 * M_PI, -0.5, 0.5),
            R(R), p0(p0), H(H) {
        ParametrisedMeshGenerator::generate();
    }

    ParameterMap getParameters() override {
        ParameterMap map {
                {"R", R},
                {"H", H},
                {"p0x", *(glm::value_ptr(p0) + 0)},
                {"p0y", *(glm::value_ptr(p0) + 1)},
                {"p0z", *(glm::value_ptr(p0) + 2)}
        };
        auto parentMap = ParametrisedMeshGenerator::getParameters();
        map.insert(map.end(),std::make_move_iterator(parentMap.begin()), std::make_move_iterator(parentMap.end()));
        return map;
    }

    std::string getName() final {
        return "Cylinder Generator";
    };
};

#endif //OPENGL_SANDBOX_CYLINDERGENERATOR_H
