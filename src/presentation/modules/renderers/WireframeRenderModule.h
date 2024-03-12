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
    Shader bezierShader;

public:
    explicit WireframeRenderModule(int workspaceWidth) : workspaceWidth(workspaceWidth),
            shader(Shader("wireframe",
                            IOUtils::getResource("shaders/basic/position.vert"),
                            IOUtils::getResource("shaders/basic/selection.frag"))),
            pointShader(Shader("point",
                           IOUtils::getResource("shaders/basic/point.vert"),
                           IOUtils::getResource("shaders/basic/selection.frag"))),
            bezierShader(Shader("bezier",
                            IOUtils::getResource("shaders/bezier/bezier.vert"),
                            IOUtils::getResource("shaders/bezier/bezier.geom"),
                            IOUtils::getResource("shaders/bezier/bezier.frag")))
                           {}

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

        bezierShader.use();
        bezierShader.setUniform("selected", false);
        bezierShader.setUniform("windowWidth", int(io.DisplaySize.x));
        bezierShader.setUniform("windowHeight", int(io.DisplaySize.y));
        RenderHelpers::setUpCamera(appState.camera, bezierShader);
        for(auto &bezier : std::views::values(appState.bezierC0Set)) {
            bezier->render(bezierShader);
        }
    };
};
#endif //OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H
