//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_APPLICATIONSTATE_H
#define OPENGL_SANDBOX_APPLICATIONSTATE_H


#include <vector>
#include "../presentation/rendering/model/ModelNode.h"

struct ApplicationState {
    ModelNode rootModelNode = ModelNode("root");
    std::vector<ModelNode*> selectedModelNodes;
};


#endif //OPENGL_SANDBOX_APPLICATIONSTATE_H
