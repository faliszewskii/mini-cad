//
// Created by faliszewskii on 12.01.24.
//

#ifndef OPENGL_SANDBOX_MATERIAL_H
#define OPENGL_SANDBOX_MATERIAL_H

#include <glm/vec4.hpp>
#include <string>
#include <optional>
#include "../../tree/SceneNode.h"
#include "../../tree/Bindable.h"

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Material : public SceneNode {
    Bindable<glm::vec4> albedo;
    std::optional<Texture> diffuseTexture;
    std::string hint;
public:
    explicit Material(std::string name, Bindable<glm::vec4> albedo = Bindable<glm::vec4>(glm::vec4(1.0f)), std::optional<Texture> diffuseTexture = {},
                      float shininess = 0, std::string hint = "");

    std::string getTypeName() override { return "Material"; };
    std::optional<Texture> getDiffuseTexture();
    glm::vec4 getAlbedo();
    float getShininess();
    float& getShininessRef();
    std::string getHint();

    virtual int acceptVisit(SceneNodeVisitor& visitor) override;
    virtual int acceptLeave(SceneNodeVisitor& visitor) override;


    float shininess;
};


#endif //OPENGL_SANDBOX_MATERIAL_H
