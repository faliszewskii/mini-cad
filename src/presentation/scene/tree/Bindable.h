//
// Created by faliszewskii on 14.01.24.
//

#ifndef OPENGL_SANDBOX_BINDABLE_H
#define OPENGL_SANDBOX_BINDABLE_H

#include <optional>
#include <functional>

template<typename T>
class Bindable {
    T value;
    std::optional<std::function<T()>> getter;
public:
    Bindable() : value(T()) {};

    Bindable(T value) : value(value) {};

    explicit Bindable(std::function<T()> getter) : value(T()), getter(getter) {};

    void bind(std::function<T()> getterToBind) { getter = getterToBind; };

    T &get() {
        if (getter) value = getter.value()();
        return value;
    };

    T get() const {
        if (getter) value = getter.value()();
        return value;
    };

    Bindable &operator=(T other) {
        value = other;
        return *this;
    };
};

#endif //OPENGL_SANDBOX_BINDABLE_H
