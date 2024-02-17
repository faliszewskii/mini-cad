//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_NORMALRENDERMODULE_H
#define OPENGL_SANDBOX_NORMALRENDERMODULE_H

#include "../Module.h"
#include "../../scene/nodes/shader/Shader.h"
#include "../../../logic/algebra/Rect.h"
#include "RenderHelpers.h"
#include "../../../logic/state/AppState.h"
#include "../../../logic/io/IOUtils.h"

class NormalRenderModule : public Module {
    const int workspaceWidth;
    Shader shader;

public:
    explicit NormalRenderModule(int workspaceWidth) : Module(false), workspaceWidth(workspaceWidth),
              shader(Shader("normalDebug",
                            IOUtils::getResource("shaders/debug/normalDebug.vert"),
                            IOUtils::getResource("shaders/debug/normalDebug.geom"),
                            IOUtils::getResource("shaders/debug/normalDebug.frag"))) {}

    void run(AppState &appState) final {
        ImGuiIO &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);
        shader.use();

        if(!appState.currentCamera) return;
        Camera &camera = appState.currentCamera.value().get();
        RenderHelpers::setUpCamera(camera, shader);
        RenderHelpers::renderMeshesNoMaterial(appState.transformTree, shader);
    };

    [[nodiscard]] std::string getName() const final {
        return "Normal Render Module";
    }
};

#endif //OPENGL_SANDBOX_NORMALRENDERMODULE_H
