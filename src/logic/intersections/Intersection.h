//
// Created by USER on 22/08/2024.
//

#ifndef INTERSECTION_H
#define INTERSECTION_H
#include <utility>
#include <vector>

#include "IntersectableSurface.h"
#include "../entities/shader/Shader.h"
#include "../vertices/PositionVertex.h"
#include "../ids/IdCounter.h"
#include "../entities/mesh/Mesh.h"
#include "../intersections/Intersectable.h"

class Intersection {
    Mesh<PositionVertex> mesh;
public:
    int id;
    std::string name;
    bool selected;

    std::vector<glm::vec3> points;
    glm::vec4 color;

    std::vector<std::pair<IntersectableSurface, std::vector<glm::vec2>>> surfaces;
    bool wrapped;

    Intersection(std::vector<glm::vec3> ipoints, std::vector<std::pair<IntersectableSurface, std::vector<glm::vec2>>> surfaces, bool wrapped) :
        id(IdCounter::nextId()), name("Intersection ("+std::to_string(id)+")"), selected(false), wrapped(wrapped), points(std::move(ipoints)),
        surfaces(std::move(surfaces)), mesh(Mesh<PositionVertex>({}, {}, GL_POINTS)) {
        std::vector<PositionVertex> vertices;
        std::transform(points.begin(), points.end(), std::back_inserter(vertices), [](glm::vec3 p) {return PositionVertex({p.x, p.y, p.z});});
        mesh.update(std::move(vertices), std::nullopt);

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<float> dist(0.2, 0.8);
        color = glm::vec4(dist(rng), dist(rng), dist(rng), 1.0f);

    }

    void render(Shader &shader) {
        shader.setUniform("selected", selected);
        shader.setUniform("model", glm::mat4(1.0f));
        shader.setUniform("color", color);
        glPointSize(4.0);
        mesh.render();
        glPointSize(1.0);
    }
};

#endif //INTERSECTION_H
