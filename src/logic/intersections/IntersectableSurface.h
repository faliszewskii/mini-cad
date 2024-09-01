//
// Created by USER on 22/08/2024.
//

#ifndef INTERSECTIONTYPE_H
#define INTERSECTIONTYPE_H
#include "Intersectable.h"
#include "ParametricSurface.h"

template<typename ...T> requires ((ParametricSurface<T> && ...) && (Intersectable<T> && ...)) using IntersectableSurfaceTemp = ReferenceVariant<T...>;
using IntersectableSurface = IntersectableSurfaceTemp<Torus, PatchC0, PatchC2>;

#endif //INTERSECTIONTYPE_H
