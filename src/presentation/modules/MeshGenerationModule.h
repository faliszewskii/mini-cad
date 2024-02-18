//
// Created by faliszewskii on 18.02.24.
//

#ifndef OPENGL_SANDBOX_MESHGENERATIONMODULE_H
#define OPENGL_SANDBOX_MESHGENERATIONMODULE_H

#include "../scene/nodes/mesh/Mesh.h"

class MeshGenerationModule {

    struct MeshGenerator {
        Mesh &target;
        std::function<Mesh()> function;
    };

    std::vector<MeshGenerator> generators;

    void test() {
        Mesh m("", std::vector<Vertex>());
        int b;
        generators.push_back(MeshGenerator(m, [](int b){ return Mesh("", std::vector<Vertex>()); }));
    };
};

#endif //OPENGL_SANDBOX_MESHGENERATIONMODULE_H
