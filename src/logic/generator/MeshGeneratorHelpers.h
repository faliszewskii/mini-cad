//
// Created by faliszewskii on 18.02.24.
//

#ifndef OPENGL_SANDBOX_MESHGENERATORHELPERS_H
#define OPENGL_SANDBOX_MESHGENERATORHELPERS_H

#include <vector>

namespace MeshGeneratorHelpers {
    inline void addQuad(std::vector<unsigned int> &indices, int i0, int i1, int i2, int i3) {
        indices.push_back(i0);
        indices.push_back(i1);
        indices.push_back(i2);

        indices.push_back(i3);
        indices.push_back(i0);
        indices.push_back(i2);
    }
}

#endif //OPENGL_SANDBOX_MESHGENERATORHELPERS_H
