//
// Created by faliszewskii on 15.02.24.
//

#ifndef OPENGL_SANDBOX_TRANSFORMTREE_H
#define OPENGL_SANDBOX_TRANSFORMTREE_H

#include <vector>
#include <memory>
#include "../../presentation/scene/nodes/transformation/Transformation.h"
#include "../../presentation/scene/nodes/mesh/Mesh.h"

struct TransformTree {
    explicit TransformTree(std::string name) : transform(std::move(name)) {};
    Transformation transform;
    std::vector<std::unique_ptr<TransformTree>> children;
    std::vector<std::unique_ptr<Mesh>> meshes;
};

#endif //OPENGL_SANDBOX_TRANSFORMTREE_H
