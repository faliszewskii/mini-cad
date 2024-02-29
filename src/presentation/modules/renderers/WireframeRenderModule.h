//
// Created by faliszewskii on 20.02.24.
//

#ifndef OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H
#define OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H

#include "../Module.h"
#include "../../../logic/io/IOUtils.h"
#include "imgui.h"
#include "RenderHelpers.h"

class WireframeRenderModule : public Module {
    const int workspaceWidth;
    Shader shader;
    Shader pointShader;

public:
    explicit WireframeRenderModule(int workspaceWidth, bool active) : Module(active), workspaceWidth(workspaceWidth),
            shader(Shader("wireframe",
                            IOUtils::getResource("shaders/debug/wireframe.vert"),
                            IOUtils::getResource("shaders/debug/wireframe.geom"),
                            IOUtils::getResource("shaders/basic/depthColor.frag"))),
           pointShader(Shader("point",
                           IOUtils::getResource("shaders/basic/point.vert"),
                           IOUtils::getResource("shaders/basic/depthColor.frag"))) {}

    void run(AppState &appState) final {
        ImGuiIO &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);

        if(!appState.currentCamera) return;
        Camera &camera = appState.currentCamera.value().get();
        RenderHelpers::setUpCamera(camera, {shader, pointShader});
        RenderHelpers::renderMeshes(appState.transformTree, {shader, pointShader}, overloaded{
            [&](std::unique_ptr<Mesh<Vertex>>& mesh) {
                shader.use(); // TODO Transform tree me not like it bo trzeba by zrobić selection viz to trzeba uniform ustawiać w trakcie przechodzenia przez drzewo, a to zrobiłem niemodyfikowalne.
                shader.setUniform("selected", appState.selectionGroup.getSelectedMesh() && (&appState.selectionGroup.getSelectedMesh()->get() == &*mesh));
                mesh->render();
            },
            [&](std::unique_ptr<MeshGenerator>& generator) {
                shader.use();
                auto &mesh = generator->getTargetMesh();
                bool t = appState.selectionGroup.getSelectedMeshGenerator() && (&appState.selectionGroup.getSelectedMeshGenerator()->get().getTargetMesh() == &mesh);
                shader.setUniform("selected", t);
                mesh.render();
            },
            [&](std::unique_ptr<Point>& point) {
                pointShader.use();
                shader.setUniform("selected", appState.selectionGroup.getSelectedPoint() && (&appState.selectionGroup.getSelectedPoint()->get() == &*point));
                point->render(pointShader);
            }
        });
    };

    [[nodiscard]] std::string getName() const final {
        return "Wireframe Render Module";
    }
};
#endif //OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H
