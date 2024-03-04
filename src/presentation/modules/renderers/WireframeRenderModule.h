//
// Created by faliszewskii on 20.02.24.
//

#ifndef OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H
#define OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H

#include "../../../logic/io/IOUtils.h"
#include "imgui.h"
#include "RenderHelpers.h"

class WireframeRenderModule {
    const int workspaceWidth;
    Shader shader;
    Shader pointShader;

public:
    explicit WireframeRenderModule(int workspaceWidth) : workspaceWidth(workspaceWidth),
            shader(Shader("wireframe",
                            IOUtils::getResource("shaders/debug/wireframe.vert"),
                            IOUtils::getResource("shaders/debug/wireframe.geom"),
                            IOUtils::getResource("shaders/basic/depthColor.frag"))),
           pointShader(Shader("point",
                           IOUtils::getResource("shaders/basic/point.vert"),
                           IOUtils::getResource("shaders/basic/depthColor.frag"))) {}

    void run(AppState &appState) {
        ImGuiIO &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);

        shader.use();
        RenderHelpers::setUpCamera(appState.camera, shader);
        shader.setUniform("selected", false);
        for(auto &torus : std::views::values(appState.torusSet))
            torus->render(shader);
        pointShader.use();
        pointShader.setUniform("selected", false);
        RenderHelpers::setUpCamera(appState.camera, pointShader);
        for(auto &point : std::views::values(appState.pointSet))
            point->render(pointShader);
    };
};
#endif //OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H
