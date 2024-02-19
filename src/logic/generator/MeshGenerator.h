//
// Created by faliszewskii on 18.02.24.
//

#ifndef OPENGL_SANDBOX_MESHGENERATOR_H
#define OPENGL_SANDBOX_MESHGENERATOR_H

#include "../../presentation/scene/nodes/mesh/Mesh.h"

class MeshGenerator {
protected:
    Mesh &target;
public:
    explicit MeshGenerator(Mesh &target) : target(target) {}

    [[nodiscard]] const Mesh& getTargetMesh() const { return target; }

    virtual void generate() = 0;

    using ParameterMap = std::vector<std::pair<std::string, std::variant<std::reference_wrapper<int>, std::reference_wrapper<float>>>>;
    virtual ParameterMap getParameters() = 0;

    virtual std::string getName() = 0;

    virtual ~MeshGenerator() = default;
    // TODO Rule of Five
};

#endif //OPENGL_SANDBOX_MESHGENERATOR_H
