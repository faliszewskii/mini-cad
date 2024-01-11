//
// Created by faliszewskii on 08.01.24.
//

#ifndef OPENGL_SANDBOX_PROPERTYVIEWVISITOR_H
#define OPENGL_SANDBOX_PROPERTYVIEWVISITOR_H


#include "../../scene/visitor/SceneNodeVisitor.h"
#include "../../properties/visitor/PropertyVisitor.h"

class PropertyViewVisitor : public PropertyVisitor {
    void visitTransformationParameter(TransformationProperty& transformation) override;
};


#endif //OPENGL_SANDBOX_PROPERTYVIEWVISITOR_H
