//
// Created by faliszewskii on 29.02.24.
//

#ifndef OPENGL_SANDBOX_VERTICALSTRIPEDLINEMODULE_H
#define OPENGL_SANDBOX_VERTICALSTRIPEDLINEMODULE_H

#include "../../../logic/entities/shader/Shader.h"
#include "../../../logic/io/IOUtils.h"
#include "../../../logic/vertices/EmptyVertex.h"
#include "../../../logic/entities/mesh/Mesh.h"
#include "../../../logic/state/AppState.h"
#include "imgui.h"

class VerticalStripedLineModule {
    const int workspaceWidth;
    Shader shader;
    Mesh<EmptyVertex> mesh;

public:
    explicit VerticalStripedLineModule(int workspaceWidth) : workspaceWidth(workspaceWidth),
        mesh(Mesh<EmptyVertex>({EmptyVertex{}}, {}, GL_POINTS)), // TODO I don't need names here. Maybe they are not neede anywhere at all.
        shader(Shader(
                        IOUtils::getResource("shaders/basic/empty.vert"),
                        IOUtils::getResource("shaders/verticalLine/verticalLine.geom"),
                        IOUtils::getResource("shaders/verticalLine/verticalLine.frag"))) {}

    void run(AppState &appState) {
        ImGuiIO &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);

        shader.use();
        Camera &camera = appState.camera;
        if(camera.stereoscopicVision) {
            glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
            auto viewLeft = camera.getViewMatrixStereo(true);
            auto projectionLeft = camera.getProjectionMatrixStereo(true);
            render(appState, viewLeft, projectionLeft);

            glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
            glDepthFunc(GL_LEQUAL);
            auto viewRight = camera.getViewMatrixStereo(false);
            auto projectionRight = camera.getProjectionMatrixStereo(false);
            render(appState, viewRight, projectionRight);

            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        } else {
            auto view = camera.getViewMatrix();
            auto projection = camera.getProjectionMatrix();
            render(appState, view, projection);
        }

    }

    void render(AppState &appState, glm::mat4 &view, glm::mat4 &projection) const {
        shader.setUniform("projection", projection);
        shader.setUniform("view", view);
        /// Animated
        shader.setUniform("time", float(glfwGetTime()));

        auto projected = glm::vec3(appState.cursorPosition.x, 0, appState.cursorPosition.z);
        renderStripedLine(projected, appState.cursorPosition, glm::vec4(1, 1, 1, 0.5));

        if(appState.selectedEntities.size() > 1)
            for(auto &el : appState.selectedEntities)
                visit(overloaded{
                        [&](Torus &torus){ renderStripedLine(appState.centerOfMassTransformation.translation, torus.transform.translation, glm::vec4(1.f, 0.8f, 0, 0.5f)); },
                        [&](Point &point){ renderStripedLine(appState.centerOfMassTransformation.translation, point.position, glm::vec4(1.f, 0.8f, 0, 0.5f)); },
                        [&](BezierC0 &_){ /* ignore */ },
                        [&](BezierC2 &_){ /* ignore */ },
                        [&](InterpolatedC2 &_){ /* ignore */ },
                        [&](PatchC0 &_) { /* ignore */ },
                        [&](PatchC2 &_) { /* ignore */ }
                }, el.second);

        /// Un-animated
        shader.setUniform("time", 0.f);
        for(auto &pBezier : appState.bezierC0Set) {
            BezierC0 &bezier = *pBezier.second;
            if(bezier.drawPolyline && !bezier.controlPoints.empty())
                for(int i=0; i<bezier.controlPoints.size()-1; i++)
                    renderStripedLine(bezier.controlPoints[i].second.get().position, bezier.controlPoints[i + 1].second.get().position);
        }
        for(auto &pBezier : appState.bezierC2Set) { // TODO Code duplication
            BezierC2 &bezier = *pBezier.second;
            if(bezier.drawPolyline && !bezier.controlPoints.empty())
                for(int i=0; i<bezier.controlPoints.size()-1; i++)
                    renderStripedLine(bezier.controlPoints[i].second.get().position, bezier.controlPoints[i + 1].second.get().position);
        }
    }

    void renderStripedLine(glm::vec3 begin, glm::vec3 end, glm::vec4 color = glm::vec4(1.0f)) const {
        shader.setUniform("beginPointPos", begin);
        shader.setUniform("endPointPos", end);
        shader.setUniform("color", color);
        mesh.render();
    }
};



#endif //OPENGL_SANDBOX_VERTICALSTRIPEDLINEMODULE_H
