//
// Created by faliszewskii on 18.02.24.
//

#ifndef OPENGL_SANDBOX_PARAMETRISEDMESHGENERATOR_H
#define OPENGL_SANDBOX_PARAMETRISEDMESHGENERATOR_H

#include "MeshGenerator.h"
#include "MeshGeneratorHelpers.h"

class ParametrisedMeshGenerator : public MeshGenerator {
protected:
    int uCount, vCount;
    // U and V constraints.
    float uMin, uMax;
    float vMin, vMax;
    // 3D equation
    virtual float x(float u, float v) = 0;
    virtual float y(float u, float v) = 0;
    virtual float z(float u, float v) = 0;
    // Partial derivative with respect to U
    virtual float xdu(float u, float v) { return 0; };
    virtual float ydu(float u, float v) { return 0; };
    virtual float zdu(float u, float v) { return 0; };
    // Partial derivative with respect to V
    virtual float xdv(float u, float v) { return 0; };
    virtual float ydv(float u, float v) { return 0; };
    virtual float zdv(float u, float v) { return 0; };

public:
    explicit ParametrisedMeshGenerator(Mesh& mesh, int uCount, int vCount, float uMin, float uMax, float vMin, float vMax):
        MeshGenerator(mesh), uCount(uCount), vCount(vCount), uMin(uMin), uMax(uMax), vMin(vMin), vMax(vMax) {}

    void generate() final;

    ParameterMap getParameters() override;
};

#endif //OPENGL_SANDBOX_PARAMETRISEDMESHGENERATOR_H
