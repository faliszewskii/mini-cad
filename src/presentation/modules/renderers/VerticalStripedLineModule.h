//
// Created by faliszewskii on 29.02.24.
//

#ifndef OPENGL_SANDBOX_VERTICALSTRIPEDLINEMODULE_H
#define OPENGL_SANDBOX_VERTICALSTRIPEDLINEMODULE_H

#include "../../../logic/entities/shader/Shader.h"
#include "../Module.h"
#include "../../../logic/io/IOUtils.h"
#include "../../../logic/vertices/EmptyVertex.h"
#include "../../../logic/entities/mesh/Mesh.h"
#include "../../../logic/state/AppState.h"
#include "imgui.h"
#include "RenderHelpers.h"

class VerticalStripedLineModule : public Module {
    const int workspaceWidth;
    Shader shader;
    Mesh<EmptyVertex> mesh;

public:
    explicit VerticalStripedLineModule(int workspaceWidth, bool active) : Module(active), workspaceWidth(workspaceWidth),
        mesh(Mesh<EmptyVertex>("Vertical Line Mesh", {EmptyVertex{}}, {}, {}, GL_POINTS)), // TODO I don't need names here. Maybe they are not neede anywhere at all.
        shader(Shader("verticalLine",
                        IOUtils::getResource("shaders/basic/empty.vert"),
                        IOUtils::getResource("shaders/verticalLine/verticalLine.geom"),
                        IOUtils::getResource("shaders/verticalLine/verticalLine.frag"))) {}

    void run(AppState &appState) override {
        ImGuiIO &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);

        if(!appState.currentCamera) return;
        RenderHelpers::setUpCamera(appState.currentCamera.value(), {shader});
        shader.setUniform("time", float(glfwGetTime()));

        renderVerticalLine(appState.cursorPosition);

        // Debug
        if(appState.selectionGroup.getSelectedTransformTree()) {
            auto &transformationNode = appState.selectionGroup.getSelectedTransformTree().value().get();
            std::vector<glm::mat4> transformationChain; // matrix transformations from first parent to the root.
            auto parent = transformationNode.getParent();
            while(parent) {
                transformationChain.push_back(parent->get().transform.getTransformation());
                parent = parent->get().getParent();
            }

            glm::mat4 mat = transformationNode.transform.getTransformation();
            for(auto &t : transformationChain)
                mat = t * mat;
            renderVerticalLine(glm::vec3(mat[3][0], mat[3][1], mat[3][2]));
        }
        //
    }

    void renderVerticalLine(glm::vec3 position) const {
        shader.setUniform("endPointPos", position);
        mesh.render();
    }


    [[nodiscard]] std::string getName() const final {
        return "Vertical Striped Line Module";
    }
};



#endif //OPENGL_SANDBOX_VERTICALSTRIPEDLINEMODULE_H
