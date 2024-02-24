//
// Created by faliszewskii on 12.01.24.
//

#ifndef OPENGL_SANDBOX_MATERIAL_H
#define OPENGL_SANDBOX_MATERIAL_H

#include <glm/vec4.hpp>
#include <string>
#include <optional>
#include "../SceneNode.h"

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Material : public SceneNode {
    glm::vec4 albedo;
    std::optional<Texture> diffuseTexture;
    float shininess;
    std::string hint;
public:
    explicit Material(std::string name, glm::vec4 albedo = glm::vec4(1.0f),
                      std::optional<Texture> diffuseTexture = {},
                      float shininess = 0, std::string hint = "");

    std::string getTypeName() override { return "Material"; };

    std::optional<Texture> getDiffuseTexture();

    glm::vec4 getAlbedo();
    glm::vec4& getAlbedoRef();

    float getShininess();
    float &getShininessRef();

    std::string getHint();
};


#endif //OPENGL_SANDBOX_MATERIAL_H
