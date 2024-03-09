//
// Created by faliszewskii on 09.03.24.
//

#ifndef OPENGL_SANDBOX_BEZIERC0_H
#define OPENGL_SANDBOX_BEZIERC0_H

#include <vector>
#include "Point.h"
#include "../vertices/PositionVertex.h"

class BezierC0 {
    Mesh<PositionVertex> mesh;
public:
    std::string name;
    bool selected;
    int id;

    std::vector<std::reference_wrapper<Point>> controlPoints;

    BezierC0() : name("Bezier C0"), selected(false), id(IdCounter::nextId()), mesh({},{},GL_LINES_ADJACENCY) {}

    void addPoint(Point &newPoint) { // TODO Add reaction to the event of deleting a point from pointSet.
        controlPoints.emplace_back(newPoint);
        std::vector<PositionVertex> vertices;
        vertices.reserve(controlPoints.size());
        for(Point &point : controlPoints) {
            vertices.emplace_back(point.position);
        }
        std::vector<unsigned int> indices;
        for(int i = 3; i < controlPoints.size(); i += 3) {
            indices.push_back(i-3);
            indices.push_back(i-2);
            indices.push_back(i-1);
            indices.push_back(i);
        }
        mesh.update(std::move(vertices), std::move(indices));
    }

    void render(Shader &shader) const {
        glLineWidth(2);
        shader.setUniform("selected", selected);
        shader.setUniform("segmentCount", 255); // TODO Adaptive segment count.
        mesh.render();
        glLineWidth(1);
    }
};

#endif //OPENGL_SANDBOX_BEZIERC0_H
