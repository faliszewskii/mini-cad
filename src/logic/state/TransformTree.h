//
// Created by faliszewskii on 15.02.24.
//

#ifndef OPENGL_SANDBOX_TRANSFORMTREE_H
#define OPENGL_SANDBOX_TRANSFORMTREE_H

#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>
#include "../entities/mesh/Mesh.h"
#include "../entities/transformation/Transformation.h"

class TransformTree {
public:
    Transformation transform;
private:
    std::optional<std::reference_wrapper<TransformTree>> parent;
    std::vector<std::unique_ptr<TransformTree>> children;
    std::vector<std::unique_ptr<Mesh>> meshes;

public:
    explicit TransformTree(std::string name) : transform(std::move(name)) {};
    explicit TransformTree(std::string name, glm::mat4 transform) : transform(std::move(name), transform) {};

    TransformTree& addChlid(std::unique_ptr<TransformTree> &&child) {
        child->parent = *this;
        children.push_back(std::move(child));
        return *children[children.size()-1];
    }

    Mesh& addChlid(std::unique_ptr<Mesh> &&mesh) {
        meshes.push_back(std::move(mesh));
        return *meshes[meshes.size()-1];
    }

    [[nodiscard]] const std::vector<std::unique_ptr<TransformTree>>& getChildren() const {
        return children;
    };

    [[nodiscard]] const std::vector<std::unique_ptr<Mesh>>& getMeshes() const {
        return meshes;
    };

    [[nodiscard]] std::optional<std::reference_wrapper<TransformTree>> getParent() const {
        return parent;
    };
};

#endif //OPENGL_SANDBOX_TRANSFORMTREE_H
