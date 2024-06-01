//
// Created by faliszewskii on 05.03.24.
//

#ifndef OPENGL_SANDBOX_ENTITYTYPE_H
#define OPENGL_SANDBOX_ENTITYTYPE_H

#include "../geometry/Torus.h"
#include "../concepts/has_id.h"
#include "../concepts/has_selected.h"
#include "../geometry/Point.h"
#include "../geometry/BezierC0.h"
#include "../geometry/BezierC2.h"
#include "../geometry/InterpolatedC2.h"
#include "../geometry/PatchC0.h"
#include "../geometry/PatchC2.h"
#include "../geometry/GregoryPatch.h"

template<typename ...RefT> using ReferenceVariant = std::variant<std::reference_wrapper<RefT>...>;
template<typename ...T> requires (has_id<T> && ...) && (has_selected<T> && ...) using EntityTypeTemp = ReferenceVariant<T...>;
using EntityType = EntityTypeTemp<Torus, Point, BezierC0, BezierC2, InterpolatedC2, PatchC0, PatchC2, GregoryPatch>;

#endif //OPENGL_SANDBOX_ENTITYTYPE_H
