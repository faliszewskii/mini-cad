//
// Created by faliszewskii on 07.01.24.
//

#include "Light.h"

#include <utility>

Light::Light(std::string name, glm::vec3 position) : SceneNode(std::move(name)), position(position) {}
