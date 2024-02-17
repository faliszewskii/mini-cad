//
// Created by faliszewskii on 15.02.24.
//

#ifndef OPENGL_SANDBOX_SELECTIONGROUP_H
#define OPENGL_SANDBOX_SELECTIONGROUP_H

#include <optional>
#include "../../presentation/scene/nodes/camera/Camera.h"
#include "../../presentation/scene/nodes/material/Material.h"
#include "../../presentation/scene/nodes/light/Light.h"
#include "../../presentation/scene/nodes/mesh/Mesh.h"
#include "TransformTree.h"


class SelectionGroup {
    #define GROUPS                          \
    GROUP(Camera, CAMERA)                   \
    GROUP(Material, MATERIAL)               \
    GROUP(TransformTree, TRANSFORM_TREE)    \
    GROUP(Light, LIGHT)                     \
    GROUP(Mesh, MESH)

    enum Focus {
        #define GROUP(T, ENUM) \
            ENUM,
        GROUPS
        #undef GROUP
        NONE
    } currentFocus = NONE;

    #define GROUP(T, ENUM) \
        std::optional<std::reference_wrapper<T>> selected##T;
    GROUPS
    #undef GROUP

    void unsetFocus(Focus focus) {
        switch(focus) {
            #define GROUP(T, ENUM)              \
                case ENUM: selected##T = {};    \
                break;
            GROUPS
            #undef GROUP
            case NONE: break;
        }
    };

public:

    #define GROUP(T, ENUM)                                                  \
        const std::optional<std::reference_wrapper<T>>& getSelected##T() {  \
            return selected##T;                                             \
        }
    GROUPS
    #undef GROUP

    #define GROUP(T, ENUM)              \
        void setFocus(T& el) {          \
            unsetFocus(currentFocus);   \
            selected##T = el;           \
            currentFocus = ENUM;        \
        };
    GROUPS
    #undef GROUP

};

#endif //OPENGL_SANDBOX_SELECTIONGROUP_H
