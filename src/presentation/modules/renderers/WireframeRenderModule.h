//
// Created by faliszewskii on 20.02.24.
//

#ifndef OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H
#define OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H

#include "../../../logic/io/IOUtils.h"
#include "imgui.h"

class WireframeRenderModule {
    const int workspaceWidth;
    Shader shader;
    Shader pointShader;
    Shader bezierShader;
    Shader bezierC2Shader;

public:
    explicit WireframeRenderModule(int workspaceWidth) : workspaceWidth(workspaceWidth),
            shader(Shader(
                            IOUtils::getResource("shaders/basic/position.vert"),
                            IOUtils::getResource("shaders/basic/selection.frag"))),
            pointShader(Shader(
                           IOUtils::getResource("shaders/basic/point.vert"),
                           IOUtils::getResource("shaders/basic/selection.frag"))),
            bezierShader(Shader(
                                IOUtils::getResource("shaders/bezier/bezier.vert"),
                                IOUtils::getResource("shaders/bezier/bezier.tesc"),
                                IOUtils::getResource("shaders/bezier/bezier.tese"),
                            IOUtils::getResource("shaders/bezier/bezier.frag"))), // TODO DEBUG change to selection.frag
            bezierC2Shader(Shader(
                                 IOUtils::getResource("shaders/bezierC2/bezierC2.vert"),
                                 IOUtils::getResource("shaders/bezierC2/bezierC2.tesc"),
                                 IOUtils::getResource("shaders/bezierC2/bezierC2.tese"),
                                 IOUtils::getResource("shaders/bezierC2/bezierC2.frag"))) // TODO DEBUG change to selection.frag
                           {}

    void run(AppState &appState) {
        ImGuiIO &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);

        Camera &camera = appState.camera;

        if(camera.stereoscopicVision) {
            glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
            auto viewLeft = camera.getViewMatrixStereo(true);
            auto projectionLeft = camera.getProjectionMatrixStereo(true);
            render(appState, io, viewLeft, projectionLeft);

            glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
            auto viewRight = camera.getViewMatrixStereo(false);
            auto projectionRight = camera.getProjectionMatrixStereo(false);
            render(appState, io, viewRight, projectionRight);

            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        } else {
            auto view = camera.getViewMatrix();
            auto projection = camera.getProjectionMatrix();
            render(appState, io, view, projection);
        }


    }

    void render(const AppState &appState, const ImGuiIO &io, glm::mat4 &view, glm::mat4 &projection) {
        shader.use();
        shader.setUniform("projection", projection);
        shader.setUniform("view", view);
        shader.setUniform("selected", false);
        shader.setUniform("color", glm::vec4(0, 0, 0, 1));
        for(auto &torus : std::views::values(appState.torusSet))
            torus->render(shader);

        pointShader.use();
        pointShader.setUniform("selected", false);
        pointShader.setUniform("color", glm::vec4(0, 0, 0, 1));
        pointShader.setUniform("projection", projection);
        pointShader.setUniform("view", view);
        for(auto &point : std::views::values(appState.pointSet))
            point->render(pointShader);

        for(auto &bezier : std::views::values(appState.bezierC2Set)) {
            if (bezier->drawBernstein) {
                for (auto &point: bezier->bernsteinPoints)
                    point->render(pointShader);
            }
        }

        bezierShader.use();
        bezierShader.setUniform("color", glm::vec4(0, 0, 0, 1));
        bezierShader.setUniform("selected", false);
        bezierShader.setUniform("windowWidth", int(io.DisplaySize.x));
        bezierShader.setUniform("windowHeight", int(io.DisplaySize.y));
        bezierShader.setUniform("projection", projection);
        bezierShader.setUniform("view", view);
        for(auto &bezier : std::views::values(appState.bezierC0Set)) {
            bezier->render(bezierShader);
        }

        bezierC2Shader.use();
        bezierC2Shader.setUniform("color", glm::vec4(0, 0, 0, 1));
        bezierC2Shader.setUniform("selected", false);
        bezierC2Shader.setUniform("windowWidth", int(io.DisplaySize.x));
        bezierC2Shader.setUniform("windowHeight", int(io.DisplaySize.y));
        bezierC2Shader.setUniform("projection", projection);
        bezierC2Shader.setUniform("view", view);
        for(auto &bezier : std::views::values(appState.bezierC2Set)) {
            bezier->render(bezierC2Shader);
        }
    };

    void hotReload() {
        shader.hotReload();
        pointShader.hotReload();
        bezierShader.hotReload();
        bezierC2Shader.hotReload();
    }
};

#endif //OPENGL_SANDBOX_WIREFRAMERENDERMODULE_H
