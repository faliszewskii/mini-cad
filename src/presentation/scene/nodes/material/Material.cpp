//
// Created by faliszewskii on 12.01.24.
//

#include "Material.h"

#include <utility>
#include <iostream>

Material::Material(std::string name, Bindable<glm::vec4> albedo, std::optional<Texture> diffuseTexture,
                   float shininess, std::string hint) :
        SceneNode(std::move(name)), albedo(std::move(albedo)), diffuseTexture(std::move(diffuseTexture)),
        shininess(shininess), hint(std::move(hint)) {}

std::optional<Texture> Material::getDiffuseTexture() {
    return diffuseTexture;
}

glm::vec4 Material::getAlbedo() {
    return albedo.get();
}

float Material::getShininess() {
    return shininess;
}

int Material::acceptVisit(SceneNodeVisitor &visitor) {
    return visitor.visitMaterial(*this);
}

int Material::acceptLeave(SceneNodeVisitor &visitor) {
    return visitor.leaveMaterial(*this);
}

std::string Material::getHint() {
    return hint;
}

float &Material::getShininessRef() {
    return shininess;
}
