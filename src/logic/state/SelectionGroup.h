//
// Created by faliszewskii on 15.02.24.
//

#ifndef OPENGL_SANDBOX_SELECTIONGROUP_H
#define OPENGL_SANDBOX_SELECTIONGROUP_H

#include <optional>
#include "../generator/MeshGenerator.h"
#include "TransformTree.h"
#include "../entities/camera/Camera.h"
#include "../entities/material/Material.h"
#include "../entities/light/Light.h"
#include "../entities/mesh/Mesh.h"


class SelectionGroup {
    #define GROUPS                          \
    GROUP(Camera, , CAMERA)                   \
    GROUP(Material, , MATERIAL)               \
    GROUP(TransformTree, , TRANSFORM_TREE)    \
    GROUP(Light, , LIGHT)                     \
    GROUP(Mesh, <Vertex>, MESH)                       \
    GROUP(MeshGenerator, , MESH_GENERATOR)

    enum Focus {
        #define GROUP(T, TEMPLATE, ENUM) \
            ENUM,
        GROUPS
        #undef GROUP
        NONE
    } currentFocus = NONE;

    #define GROUP(T, TEMPLATE, ENUM) \
        std::optional<std::reference_wrapper<T TEMPLATE>> selected##T;
    GROUPS
    #undef GROUP

    void unsetFocus(Focus focus) {
        switch(focus) {
            #define GROUP(T, TEMPLATE, ENUM)              \
                case ENUM: selected##T = {};    \
                break;
            GROUPS
            #undef GROUP
            case NONE: break;
        }
    };

public:

    #define GROUP(T, TEMPLATE, ENUM)                                                  \
        const std::optional<std::reference_wrapper<T TEMPLATE>>& getSelected##T() {  \
            return selected##T;                                             \
        }
    GROUPS
    #undef GROUP

    #define GROUP(T, TEMPLATE, ENUM)    \
        void setFocus(T TEMPLATE& el) {          \
            unsetFocus(currentFocus);   \
            selected##T = el;           \
            currentFocus = ENUM;        \
        };
    GROUPS
    #undef GROUP

};

#endif //OPENGL_SANDBOX_SELECTIONGROUP_H
