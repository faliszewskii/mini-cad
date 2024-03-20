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
    Mesh<SplineVertex> mesh;
public:
    int id;
    std::string name;
    bool selected;

    bool drawPolyline;
    int adaptationMultiplier=20;
    std::vector<std::pair<int, std::reference_wrapper<Point>>> controlPoints;

    BezierC2() : id(IdCounter::nextId()), name("Bezier C2  ("+std::to_string(id)+")"), selected(false), mesh({},{},GL_PATCHES),
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
        mesh.update({point.position, static_cast<float>(i)}, i);
    }

    void updateMesh() {
        std::vector<SplineVertex> vertices;
        vertices.reserve(controlPoints.size());
        for(int i=0; i<controlPoints.size(); i++) {
            auto &point = controlPoints[i];
            vertices.emplace_back(point.second.get().position, i);
        }
        std::vector<unsigned int> indices;
        int s = controlPoints.size();
        for(int i = 0; i < s; i ++) {
            indices.push_back(i-3 >= 0 ? i-3 : 0);
            indices.push_back(i-2 >= 0 ? i-2 : 0);
            indices.push_back(i-1 >= 0 ? i-1 : 0);
            indices.push_back(i);
            indices.push_back(i+1 < s ? i+1 : s-1);
            indices.push_back(i+2 < s ? i+2 : s-1);
            indices.push_back(i+3 < s ? i+3 : s-1);
            // TODO Add padding on the right
        }
//        for(int i = 0; i < s; i ++) {
//            indices.push_back(i-2>=0? i-2 : 0);
//            indices.push_back(i-1>=0? i-1 : 0);
//            indices.push_back(i);
//            indices.push_back(i+1<s? i+1 : s-1);
//            indices.push_back(i+2<s? i+2 : s-1);
//        }
        mesh.update(std::move(vertices), std::move(indices));
    }

    void render(Shader &shader) {
        glLineWidth(2);
        shader.setUniform("adaptationMultiplier", adaptationMultiplier);
        glPatchParameteri(GL_PATCH_VERTICES, 7);
        mesh.render();
        glLineWidth(1);
    }
};


#endif //OPENGL_SANDBOX_BEZIERC2_H
