//
// Created by faliszewskii on 29.02.24.
//

#ifndef OPENGL_SANDBOX_IS_VERTEX_H
#define OPENGL_SANDBOX_IS_VERTEX_H

template<typename T>
concept is_vertex = requires(T v) {
    { T::getSizes() } -> std::same_as<std::vector<int>>;
    { T::getTypes() } -> std::same_as<std::vector<int>>;
    { T::getOffsets() } -> std::same_as<std::vector<int>>;
};

#endif //OPENGL_SANDBOX_IS_VERTEX_H
