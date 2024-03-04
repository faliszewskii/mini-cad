//
// Created by faliszewskii on 03.03.24.
//

#ifndef OPENGL_SANDBOX_HAS_ID_H
#define OPENGL_SANDBOX_HAS_ID_H

#include <concepts>

template<typename T> concept has_id = requires (T a) { {a.id} -> std::same_as<int&>; };

#endif //OPENGL_SANDBOX_HAS_ID_H
