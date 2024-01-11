//
// Created by faliszewskii on 10.01.24.
//

#ifndef OPENGL_SANDBOX_PROPERTY_H
#define OPENGL_SANDBOX_PROPERTY_H


#include "visitor/PropertyVisitor.h"

class Property {
public:
    virtual std::string getPropertyName() = 0;

    virtual void acceptVisit(PropertyVisitor& visitor) = 0;
};


#endif //OPENGL_SANDBOX_PROPERTY_H
