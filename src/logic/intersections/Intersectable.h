//
// Created by USER on 22/08/2024.
//

#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H
#include <vector>
#include <glm/vec2.hpp>

class Texture;
template<typename T>
concept Intersectable = requires(T intersectable, int id, std::vector<glm::vec2> intersection,
    std::array<std::array<glm::vec3, 256>, 256> mask, float u, float v) {
    { intersectable.setMask(mask) } -> std::same_as<void>;
    { intersectable.getMask() } -> std::same_as<std::pair<std::reference_wrapper<Texture>, std::reference_wrapper<std::array<std::array<glm::vec3, 256>, 256>>>>;
    { intersectable.clearMask() } -> std::same_as<void>;
};

#endif //INTERSECTABLE_H
