//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_RANDOMRENDERMODULE_H
#define OPENGL_SANDBOX_RANDOMRENDERMODULE_H

#include "../Module.h"
#include "../../../logic/algebra/Rect.h"
#include "RenderHelpers.h"
#include "../../../logic/state/AppState.h"
#include "../../../logic/io/IOUtils.h"

class RandomRenderModule : public Module {
    const int workspaceWidth;
    Shader shader;

public:
    explicit RandomRenderModule(int workspaceWidth, bool active) : Module(active), workspaceWidth(workspaceWidth),
                                                      shader(Shader("random",
                         IOUtils::getResource("shaders/basic/white.vert"),
                         IOUtils::getResource("shaders/debug/random.frag"))) {}

    void run(AppState &appState) final {
        ImGuiIO &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);
        shader.use();

        if(!appState.currentCamera) return;
        Camera &camera = appState.currentCamera.value().get();
        RenderHelpers::setUpCamera(camera, {shader});
        RenderHelpers::renderMeshes(appState.transformTree, {shader}, overloaded{
                [&](std::unique_ptr<Mesh<Vertex>>& mesh) {
                    shader.use();
                    mesh->render();
                },
                [&](std::unique_ptr<MeshGenerator>& generator) {
                    shader.use();
                    auto &mesh = generator->getTargetMesh();
                    mesh.render();
                },
                [&](std::unique_ptr<Point>& point) {
                    shader.use();
                    point->render(shader);
                }
        });
    };

    [[nodiscard]] std::string getName() const final {
        return "Random Render Module";
    }
};

#endif //OPENGL_SANDBOX_RANDOMRENDERMODULE_H
