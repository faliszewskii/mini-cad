//
// Created by faliszewskii on 18.02.24.
//

#ifndef OPENGL_SANDBOX_MESHGENERATOR_H
#define OPENGL_SANDBOX_MESHGENERATOR_H

#include "../entities/mesh/Mesh.h"

class MeshGenerator {
protected:
    std::unique_ptr<Mesh<Vertex>> target;
public:
    explicit MeshGenerator() : target(std::make_unique<Mesh<Vertex>>("Generated")) {}

    [[nodiscard]] Mesh<Vertex>& getTargetMesh() const { return *target; }

    virtual void generate() = 0;

    using ParameterMap = std::vector<std::pair<std::string, std::variant<std::reference_wrapper<int>, std::reference_wrapper<float>>>>;
    virtual ParameterMap getParameters() = 0;

    virtual std::string getName() = 0;

    virtual ~MeshGenerator() = default;
    // TODO Rule of Five
};

#endif //OPENGL_SANDBOX_MESHGENERATOR_H
