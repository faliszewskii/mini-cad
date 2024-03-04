//
// Created by faliszewskii on 03.03.24.
//

#ifndef OPENGL_SANDBOX_HAS_NAME_H
#define OPENGL_SANDBOX_HAS_NAME_H

#include <string>

template<typename T> concept has_name = requires (T a) { {a.name} -> std::same_as<std::string&>; };

#endif //OPENGL_SANDBOX_HAS_NAME_H
