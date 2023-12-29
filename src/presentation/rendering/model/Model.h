//
// Created by faliszewskii on 28.12.23.
//

#ifndef OPENGL_SANDBOX_MODEL_H
#define OPENGL_SANDBOX_MODEL_H


#include <assimp/scene.h>
#include <glm/gtx/quaternion.hpp>
#include "../shader/shader.h"
#include "Mesh.h"
#include "ModelNode.h"

class Model
{
public:
    Model(ModelNode rootNode);
    void draw(Shader &shader);
    ~Model();
private:
    ModelNode rootNode;
    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scale;
};

#endif //OPENGL_SANDBOX_MODEL_H
