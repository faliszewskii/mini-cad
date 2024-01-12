//
// Created by faliszewskii on 12.01.24.
//

#include "Material.h"

#include <utility>

Material::Material(std::string name, const glm::vec4 &albedo, std::optional<Texture> diffuseTexture) : SceneNode(std::move(name)),
albedo(albedo), diffuseTexture(std::move(diffuseTexture)) {}

std::optional<Texture> Material::getDiffuseTexture() {
    return diffuseTexture;
}

glm::vec4 Material::getAlbedo() {
    return albedo;
}

int Material::acceptVisit(SceneNodeVisitor &visitor) {
    return visitor.visitMaterial(*this);
}

int Material::acceptLeave(SceneNodeVisitor &visitor) {
    return visitor.leaveMaterial(*this);
}
