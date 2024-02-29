//
// Created by faliszewskii on 29.02.24.
//

#ifndef OPENGL_SANDBOX_EMPTYVERTEX_H
#define OPENGL_SANDBOX_EMPTYVERTEX_H

#include <vector>

struct EmptyVertex{
    inline static std::vector<int> getSizes() { return {}; }
    inline static std::vector<int> getTypes() { return {}; }
    inline static std::vector<int> getOffsets() { return {}; }
};

#endif //OPENGL_SANDBOX_EMPTYVERTEX_H
