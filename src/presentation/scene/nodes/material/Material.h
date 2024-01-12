//
// Created by faliszewskii on 12.01.24.
//

#ifndef OPENGL_SANDBOX_MATERIAL_H
#define OPENGL_SANDBOX_MATERIAL_H

#include <glm/vec4.hpp>
#include <string>
#include <optional>
#include "../../tree/SceneNode.h"

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Material : public SceneNode {
public:
    explicit Material(std::string name, const glm::vec4 &albedo = glm::vec4(1.0f), std::optional<Texture> diffuseTexture = {});

    std::string getTypeName() override { return "Material"; };
    std::optional<Texture> getDiffuseTexture();
    glm::vec4 getAlbedo();

    virtual int acceptVisit(SceneNodeVisitor& visitor) override;
    virtual int acceptLeave(SceneNodeVisitor& visitor) override;
private:
    glm::vec4 albedo;
    std::optional<Texture> diffuseTexture;
};


#endif //OPENGL_SANDBOX_MATERIAL_H
