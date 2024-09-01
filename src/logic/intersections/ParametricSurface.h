//
// Created by faliszewskii on 03.08.24.
//

#ifndef OPENGL_SANDBOX_PARAMETRICSURFACE_H
#define OPENGL_SANDBOX_PARAMETRICSURFACE_H

#include <concepts>
#include <glm/vec3.hpp>
#include <array>
#include <vector>
#include "../state/EntityType.h"

// Concept for a parametric surface
template<typename T>
concept ParametricSurface = requires(T surface, float u, float v) {
    { surface.evaluate(u, v) } -> std::convertible_to<glm::vec3>;
    { surface.evaluateDU(u, v) } -> std::convertible_to<glm::vec3>;
    { surface.evaluateDV(u, v) } -> std::convertible_to<glm::vec3>;
    { surface.rangeU() } -> std::convertible_to<float>;
    { surface.rangeV() } -> std::convertible_to<float>;
    { surface.wrapU() } -> std::convertible_to<bool>;
    { surface.wrapV() } -> std::convertible_to<bool>;
};

#endif //OPENGL_SANDBOX_PARAMETRICSURFACE_H
