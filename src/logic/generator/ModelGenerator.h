//
// Created by faliszewskii on 29.12.23.
//

#ifndef OPENGL_SANDBOX_MODELGENERATOR_H
#define OPENGL_SANDBOX_MODELGENERATOR_H


#include <glm/glm.hpp>
#include "../../presentation/scene/nodes/mesh/Mesh.h"
#include "../../presentation/scene/tree/StepTree.h"
#include "../Scene.h"

class ModelGenerator {
public:
    static std::unique_ptr<Scene> generateAxis(glm::vec3 xAxis = glm::vec3(1, 0, 0), glm::vec3 yAxis = glm::vec3(0, 1, 0), bool isRightHanded = true);

    static std::unique_ptr<Scene> generateLine(std::string name, glm::vec3 start, glm::vec3 end, glm::vec4 color, const std::string &materialName);

    static Mesh generateSphereMesh(int meridianCount, int parallelCount);
    static std::unique_ptr<Scene> generateSphere(int meridianCount, int parallelCount);

    static Mesh generateTorusMesh(int radial_resolution, int tubular_resolution, float thickness = 0.25, float radius = 1);
    static std::unique_ptr<Scene> generateTorus(int radial_resolution, int tubular_resolution, float thickness = 0.25, float radius = 1);

    static std::unique_ptr<Scene> generateSolid(Mesh &&mesh, const std::string& name);

    static void addQuad(std::vector<unsigned int> &indices, int i0, int i1, int i2, int i3);

    static std::unique_ptr<Mesh> generatePlaneMesh(glm::vec3 normal);

    /// Template class design pattern for creating different parametrised models.
    class ParametrisedModelGenerator {
    protected:
        ParametrisedModelGenerator(float uMin, float uMax, float vMin, float vMax):
                uMin(uMin), uMax(uMax), vMin(vMin), vMax(vMax) {}
        // U and V constraints.
        float uMin, uMax;
        float vMin, vMax;
        // 3D equation
        virtual float x(float u, float v) { return 0; };
        virtual float y(float u, float v) { return 0; };
        virtual float z(float u, float v) { return 0; };
        // Partial derivative with respect to U
        virtual float xdu(float u, float v) { return 0; };
        virtual float ydu(float u, float v) { return 0; };
        virtual float zdu(float u, float v) { return 0; };
        // Partial derivative with respect to V
        virtual float xdv(float u, float v) { return 0; };
        virtual float ydv(float u, float v) { return 0; };
        virtual float zdv(float u, float v) { return 0; };

    public:
        Mesh generateParametrisedMesh(const std::string& name, int uCount, int vCount);
    };

    /// Implementation of the template class generating Hyperbolic Paraboloid.
    class HyperbolicParaboloidGenerator : public ParametrisedModelGenerator {
        float a, b, c;
    public:
        HyperbolicParaboloidGenerator(float a, float b, float c):
            ParametrisedModelGenerator(0, 1, 0, 1), a(a), b(b), c(c) {}

        float x(float u, float) override { return a * u; };
        float y(float, float v) override { return b * v; };
        float z(float u, float v) override { return c * u * v; };
        float xdu(float u, float) override { return a; };
        float ydu(float, float v) override { return b * v; };
        float zdu(float u, float v) override { return c * v; };
        float xdv(float u, float) override { return a * u; };
        float ydv(float, float v) override { return b; };
        float zdv(float u, float v) override { return c * u; };
    };
};


#endif //OPENGL_SANDBOX_MODELGENERATOR_H
