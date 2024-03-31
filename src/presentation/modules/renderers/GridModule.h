//
// Created by faliszewskii on 19.02.24.
//

#ifndef OPENGL_SANDBOX_GRIDMODULE_H
#define OPENGL_SANDBOX_GRIDMODULE_H

#include "../../../logic/state/AppState.h"
#include "../../../logic/io/IOUtils.h"
#include "../../../logic/vertices/Vertex.h"
#include "../../../logic/geometry/GeometryHelpers.h"


class GridModule {
    const int workspaceWidth;
    Shader shader;
    std::unique_ptr<Mesh<Vertex>> mesh;
public:
    explicit GridModule(int workspaceWidth) : workspaceWidth(workspaceWidth),
                   shader(Shader(IOUtils::getResource("shaders/grid/grid.vert"), IOUtils::getResource("shaders/grid/grid.frag"))){
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        auto normal = glm::vec3(0,1,0);

        vertices.push_back(Vertex(glm::vec3(1,1,0), glm::vec3(0,1,0)));
        vertices.push_back(Vertex(glm::vec3(1,-1,0), glm::vec3(0,1,0)));
        vertices.push_back(Vertex(glm::vec3(-1,-1,0), glm::vec3(0,1,0)));
        vertices.push_back(Vertex(glm::vec3(-1,1,0), glm::vec3(0,1,0)));

        GeometryHelpers::addQuad(indices,0, 1, 2, 3);

        mesh = std::make_unique<Mesh<Vertex>>(Mesh<Vertex>(vertices, indices, GL_TRIANGLES));
    }

    void run(AppState &appState) {
        auto &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);
        Camera &camera = appState.camera;
        auto view = camera.getViewMatrix();
        auto projection = camera.getProjectionMatrix();
        render(camera, view, projection);
    }

    void render(Camera &camera, glm::mat4 &view, glm::mat4 &projection) const {
        shader.use();
        shader.setUniform("near", camera.nearPlane);
        shader.setUniform("far", camera.farPlane);
        shader.setUniform("view", view);
        shader.setUniform("projection", projection);
        mesh->render();
    }
};

#endif //OPENGL_SANDBOX_GRIDMODULE_H
