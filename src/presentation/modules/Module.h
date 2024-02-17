//
// Created by faliszewskii on 12.02.24.
//

#ifndef OPENGL_SANDBOX_MODULE_H
#define OPENGL_SANDBOX_MODULE_H


#include <string>

class AppState;

struct Module {
    explicit Module(bool active) : active(active) {};
    bool active = true;
    [[nodiscard]] virtual std::string getName() const = 0;
    virtual void run(AppState &appState) = 0;
    virtual ~Module() = default;
    // TODO Rule of Five
};


#endif //OPENGL_SANDBOX_MODULE_H
