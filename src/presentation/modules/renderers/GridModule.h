//
// Created by faliszewskii on 19.02.24.
//

#ifndef OPENGL_SANDBOX_GRIDMODULE_H
#define OPENGL_SANDBOX_GRIDMODULE_H

#include "../Module.h"
#include "../../../logic/state/AppState.h"
#include "../../../logic/io/IOUtils.h"
#include "../../../logic/generator/MeshGeneratorHelpers.h"


class GridModule : public Module {
    const int workspaceWidth;
    Shader shader;
    std::unique_ptr<Mesh<Vertex>> mesh;
public:
    explicit GridModule(int workspaceWidth, bool active) : Module(active), workspaceWidth(workspaceWidth),
                   shader(Shader("Grid", IOUtils::getResource("shaders/grid/grid.vert"), IOUtils::getResource("shaders/grid/grid.frag"))){
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        auto normal = glm::vec3(0,1,0);

        vertices.push_back(Vertex(glm::vec3(1,1,0), glm::vec3(0,1,0)));
        vertices.push_back(Vertex(glm::vec3(1,-1,0), glm::vec3(0,1,0)));
        vertices.push_back(Vertex(glm::vec3(-1,-1,0), glm::vec3(0,1,0)));
        vertices.push_back(Vertex(glm::vec3(-1,1,0), glm::vec3(0,1,0)));

        MeshGeneratorHelpers::addQuad(indices,0, 1, 2, 3);

        mesh = std::make_unique<Mesh<Vertex>>(Mesh<Vertex>("Plane mesh", vertices, indices, {}, GL_TRIANGLES));
    }

    void run(AppState &appState) override {
        if(!appState.currentCamera) return;
        auto &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);
        Camera &camera = appState.currentCamera->get();
        shader.use();
        shader.setUniform("near", camera.nearPlane);
        shader.setUniform("far", camera.farPlane);
        shader.setUniform("view", camera.getViewMatrix());
        shader.setUniform("projection", camera.getProjectionMatrix());
        mesh->render();
    }

    [[nodiscard]] std::string getName() const override {
        return "Grid Module";
    }
};

#endif //OPENGL_SANDBOX_GRIDMODULE_H
