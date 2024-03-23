//
// Created by faliszewskii on 14.03.24.
//

#ifndef OPENGL_SANDBOX_BEZIERC2_H
#define OPENGL_SANDBOX_BEZIERC2_H

#include <vector>
#include <algorithm>
#include "Point.h"
#include "../vertices/SplineVertex.h"

class BezierC2 {
    Mesh<PositionVertex> mesh;
public:
    int id;
    std::string name;
    bool selected;

    bool drawPolyline;
    int adaptationMultiplier=20;
    std::vector<std::pair<int, std::reference_wrapper<Point>>> controlPoints;

    BezierC2() : id(IdCounter::nextId()), name("Bezier C2 ("+std::to_string(id)+")"), selected(false), mesh({},{},GL_PATCHES),
                 drawPolyline(false) {}

    bool pointAlreadyAdded(Point &point) {
        return std::ranges::any_of(controlPoints, [&](auto &controlPoint){return controlPoint.first == point.id; });
    }

    void addPoint(Point &newPoint) { // TODO Add reaction to the event of deleting a point from pointSet.
        if(pointAlreadyAdded(newPoint)) return;
        controlPoints.emplace_back(newPoint.id, newPoint);
        updateMesh();
    }

    void removePoint(int i) {
        controlPoints.erase(controlPoints.begin() + i);
        updateMesh();
    }

    void updatePoint(Point &point, int i) {
        mesh.update({point.position}, i);
    }

    void updateMesh() {
        std::vector<PositionVertex> vertices;
        vertices.reserve(controlPoints.size());
        for(auto & point : controlPoints) {
            vertices.emplace_back(point.second.get().position);
        }
        std::vector<unsigned int> indices;
        auto s = controlPoints.size();
        for(int i = 3; i < s; i ++) {
            indices.push_back(i-3);
            indices.push_back(i-2);
            indices.push_back(i-1);
            indices.push_back(i);
        }
        mesh.update(std::move(vertices), std::move(indices));
    }

    void render(Shader &shader) {
        glLineWidth(2);
        shader.setUniform("adaptationMultiplier", adaptationMultiplier);
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        mesh.render();
        glLineWidth(1);
    }
};


#endif //OPENGL_SANDBOX_BEZIERC2_H
