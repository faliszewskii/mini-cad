//
// Created by faliszewskii on 15.02.24.
//

#ifndef OPENGL_SANDBOX_VARIANTUTIL_H
#define OPENGL_SANDBOX_VARIANTUTIL_H

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

template<typename ...RefT> using ReferenceVariant = std::variant<std::reference_wrapper<RefT>...>;

#endif //OPENGL_SANDBOX_VARIANTUTIL_H
