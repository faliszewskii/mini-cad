//
// Created by faliszewskii on 14.03.24.
//

#ifndef OPENGL_SANDBOX_BEZIERC2_H
#define OPENGL_SANDBOX_BEZIERC2_H

#include <vector>
#include <algorithm>
#include "Point.h"
#include "../vertices/SplineVertex.h"
#include "../events/point/PointMovedEvent.h"

class BezierC2 {
    Mesh<PositionVertex> mesh;
public:
    int id;
    std::string name;
    bool selected;

    bool drawPolyline = true;
    int adaptationMultiplier=20;
    std::vector<std::pair<int, std::reference_wrapper<Point>>> controlPoints;

    bool drawBernstein = true;
    std::vector<std::unique_ptr<Point>> bernsteinPoints;

    BezierC2() : id(IdCounter::nextId()), name("Bezier C2 ("+std::to_string(id)+")"), selected(false), mesh({},{},GL_PATCHES) {}

    bool pointAlreadyAdded(Point &point) {
        return std::ranges::any_of(controlPoints, [&](auto &controlPoint){return controlPoint.first == point.id; });
    }

    void recalculateBernsteinPoints() {
        if(controlPoints.size() >= 4) { // First point
            auto p0 = controlPoints[0].second.get().position;
            auto p1 = controlPoints[1].second.get().position;
            auto p2 = controlPoints[2].second.get().position;

            auto fi = 2.f/3 * p1 + 1.f/3 * p2;
            auto giSub1 = 1.f/3 * p0 + 2.f/3 * p1;
            auto ei = 1.f/2 * giSub1 + 1.f/2 * fi;

            bernsteinPoints[0]->position = ei;
        }
        for(int i=3; i< controlPoints.size(); i++) { // Rest of the points
            auto p0 = controlPoints[i-3].second.get().position;
            auto p1 = controlPoints[i-2].second.get().position;
            auto p2 = controlPoints[i-1].second.get().position;
            auto p3 = controlPoints[i].second.get().position;

            auto fi = 2.f/3 * p1 + 1.f/3 * p2;
            auto fiPlus1 = 2.f/3 * p2 + 1.f/3 * p3;
            auto gi = 1.f/3 * p1 + 2.f/3 * p2;
            auto eiPlus1 = 1.f/2 * gi + 1.f /2 * fiPlus1;

            bernsteinPoints[3*(i-3)+1]->position = fi;
            bernsteinPoints[3*(i-3)+2]->position = gi;
            bernsteinPoints[3*(i-3)+3]->position = eiPlus1;

        }
    }

    void addPoint(Point &newPoint) { // TODO Add reaction to the event of deleting a point from pointSet.
        if(pointAlreadyAdded(newPoint)) return;
        controlPoints.emplace_back(newPoint.id, newPoint);
        if(controlPoints.size() >= 4) {
            const static glm::vec4 bernsteinPointColor = glm::vec4(0.2f,0,0.4f, 1);
            const static int bernsteinPointSize = 8;
            if(controlPoints.size() == 4) bernsteinPoints.push_back(std::make_unique<Point>(glm::vec3(), bernsteinPointSize, bernsteinPointColor));
            bernsteinPoints.push_back(std::make_unique<Point>(glm::vec3(), bernsteinPointSize, bernsteinPointColor));
            bernsteinPoints.push_back(std::make_unique<Point>(glm::vec3(), bernsteinPointSize, bernsteinPointColor));
            bernsteinPoints.push_back(std::make_unique<Point>(glm::vec3(), bernsteinPointSize, bernsteinPointColor));
        }
        updateMesh();
        recalculateBernsteinPoints();
    }

    void removePoint(int i) {
        controlPoints.erase(controlPoints.begin() + i);

        // Bernstein Points
        for(int i = 0; i< 3; i++) bernsteinPoints.pop_back();
        if(controlPoints.size() < 4) bernsteinPoints.pop_back();
        recalculateBernsteinPoints();

        updateMesh();
    }

    void updatePoint(Point &point, int i) {
        mesh.update({point.position}, i);
        recalculateBernsteinPoints();
    }

    void updateBernstein(Point &point, int i, glm::vec3 delta, auto &eventPublisher) {
        // Get B-Spline index based on Bernstein index. 2,3,4 -> 2
        int bSplineId = (i + 4) / 3;
        Point &bSplinePoint = controlPoints[bSplineId].second;
        Point &bSplineBeforePoint = controlPoints[bSplineId-1].second;
        Point &bSplineNextPoint = controlPoints[bSplineId+1].second;

        glm::vec3 farPoint;
        switch(i%3) {
            case 0:
                farPoint = (bSplineBeforePoint.position + bSplineNextPoint.position) / 2.f;
                break;
            case 1:
                farPoint = bSplineNextPoint.position;
                break;
            case 2:
                farPoint = bSplineBeforePoint.position;
        }
        float ratio = glm::length(bSplinePoint.position - farPoint) / glm::length(point.position - delta - farPoint);
        glm::vec3 d = delta * ratio;
        bSplinePoint.position += d;

        eventPublisher.publish(PointMovedEvent{bSplinePoint, d});
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
