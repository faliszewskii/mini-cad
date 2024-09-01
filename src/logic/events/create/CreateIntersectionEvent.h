//
// Created by USER on 22/08/2024.
//

#ifndef CREATEINTERSECTIONEVENT_H
#define CREATEINTERSECTIONEVENT_H
#include "../../intersections/IntersectableSurface.h"

struct CreateIntersectionEvent {
    std::vector<glm::vec3> intersectionPoints;
    std::vector<std::pair<IntersectableSurface, std::vector<glm::vec2>>> surfaces;
    bool wrapped;
};

#endif //CREATEINTERSECTIONEVENT_H
