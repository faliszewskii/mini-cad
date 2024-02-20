//
// Created by faliszewskii on 20.02.24.
//

#ifndef OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H
#define OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H

#include "../Module.h"
#include "../../scene/nodes/shader/Shader.h"
#include "../../../logic/io/IOUtils.h"
#include "imgui.h"
#include "RenderHelpers.h"

class WireframeRenderModule : public Module {
    const int workspaceWidth;
    Shader shader;

public:
    explicit WireframeRenderModule(int workspaceWidth) : Module(true), workspaceWidth(workspaceWidth),
                                                      shader(Shader("random",
                                                                    IOUtils::getResource("shaders/debug/wireframe.vert"),
                                                                    IOUtils::getResource("shaders/debug/wireframe.geom"),
                                                                    IOUtils::getResource("shaders/basic/depthColor.frag"))) {}

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
        return "Wireframe Render Module";
    }
};
#endif //OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H
