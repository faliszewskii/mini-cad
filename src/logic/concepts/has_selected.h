//
// Created by faliszewskii on 04.03.24.
//

#ifndef OPENGL_SANDBOX_HAS_SELECTED_H
#define OPENGL_SANDBOX_HAS_SELECTED_H

#include <concepts>

template<typename T> concept has_selected = requires (T a) { {a.selected} -> std::same_as<bool&>; };

#endif //OPENGL_SANDBOX_HAS_SELECTED_H
