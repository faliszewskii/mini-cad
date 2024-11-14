//
// Created by faliszewskii on 08.04.24.
//

#ifndef OPENGL_SANDBOX_INTERPOLATEDC2_H
#define OPENGL_SANDBOX_INTERPOLATEDC2_H


#include <string>
#include <algorithm>
#include "Point.h"
#include "../vertices/PositionVertex.h"
#include "../math/algebra/AlgebraUtils.h"

class InterpolatedC2 {
    Mesh<PositionVertex> mesh;
public:
    int id;
    std::string name;
    bool selected;

    std::vector<std::pair<int, std::reference_wrapper<Point>>> controlPoints;
    std::vector<glm::vec3> bernsteinPoints;
    int adaptationMultiplier=20;
    int instanceCount=10;

    InterpolatedC2() : id(IdCounter::nextId()), name("Interpolated C2 ("+std::to_string(id)+")"), selected(false), mesh({},{},GL_PATCHES) {}

    void recalculateBernsteinPoints() {
        bernsteinPoints.clear();
        int cpCount = controlPoints.size();
        int curveCount = cpCount - 1;

        if(cpCount < 2) return;
        if(cpCount == 2) {
            bernsteinPoints.push_back(controlPoints[0].second.get().position);
            bernsteinPoints.push_back(controlPoints[0].second.get().position);
            bernsteinPoints.push_back(controlPoints[1].second.get().position);
            bernsteinPoints.push_back(controlPoints[1].second.get().position);
            return;
        }

        std::vector<glm::vec3> interpolationPoints;
        interpolationPoints.push_back(controlPoints[0].second.get().position);
        std::vector<float> diffs;
        for(int i=0; i< curveCount; i++) {
            float f = glm::length(controlPoints[i+1].second.get().position - controlPoints[i].second.get().position);
//            diffs[i] = std::abs(diffs[i]) > 0.001f ? diffs[i] : 0.001f;
            if( std::abs(f) > 0.001f) {
                interpolationPoints.push_back(controlPoints[i+1].second.get().position);
                diffs.push_back(f);
            }
        }
//        diffs[curveCount] = 1.0f;

        cpCount = diffs.size()+1;// controlPoints.size();
        curveCount = cpCount - 1;
        int n = cpCount - 1;
        int eqCount = cpCount - 2;

        std::vector<float> a, b(eqCount, 2), c;
        std::vector<glm::vec3> d;
        for(int i=2; i<=n-1;i++) // Alphas for i=2,..., N-1
            a.push_back(diffs[i-1] / (diffs[i] + diffs[i-1]));
        for(int i=1; i<=n-2;i++) // Betas for i=1,..., N-2
            c.push_back(diffs[i] / (diffs[i] + diffs[i-1]));
        for(int i=1; i<=n-1;i++) // Rs for i=1,..., N-1
            d.push_back(3.f * ((interpolationPoints[i+1] - interpolationPoints[i])/diffs[i] - (interpolationPoints[i] - interpolationPoints[i-1])/diffs[i-1]) / (diffs[i]+diffs[i-1]));

        auto result = AlgebraUtils::solveTridiagonal(a, b, c, d);

        bernsteinPoints.resize(curveCount * 4);
        for(int i=0; i< curveCount; i++) {
            bernsteinPoints[i*4 + 0] = interpolationPoints[i];
        }

        bernsteinPoints[0*4 + 2] = glm::vec3(0);
        for(int i=1; i< curveCount; i++) {
            bernsteinPoints[i*4 + 2] = result[i-1];
        }

        for(int i=0; i< curveCount-1; i++) {
            bernsteinPoints[i*4 + 3] = (bernsteinPoints[(i+1)*4 + 2] - bernsteinPoints[i*4 + 2]) / 3.f / diffs[i];
        }
        bernsteinPoints[(curveCount-1)*4 + 3] = (glm::vec3(0) - bernsteinPoints[(curveCount-1)*4 + 2]) / 3.f / diffs[curveCount-1];

        for(int i=0; i< curveCount-1; i++) {
            bernsteinPoints[i*4 + 1] = (bernsteinPoints[(i+1)*4 + 0] - bernsteinPoints[i*4 + 0] - bernsteinPoints[i*4+2] * diffs[i] * diffs[i] - bernsteinPoints[i*4+3] * diffs[i] * diffs[i] * diffs[i] ) / diffs[i];
        }
        bernsteinPoints[(curveCount-1)*4 + 1] = (interpolationPoints[curveCount] - bernsteinPoints[(curveCount-1)*4 + 0] - bernsteinPoints[(curveCount-1)*4+2] * diffs[(curveCount-1)] * diffs[(curveCount-1)] - bernsteinPoints[(curveCount-1)*4+3] * diffs[(curveCount-1)] * diffs[(curveCount-1)] * diffs[(curveCount-1)] ) / diffs[(curveCount-1)];

        for(int i=0; i<curveCount; i++) {
            float dist = diffs[i];
            glm::mat4 bernsteinToPower(
                    1.0f, -3.0f / dist, 3.0f / powf(dist, 2.0f), -1.0f / powf(dist, 3.0f),
                    0.0f, 3.0f / dist, -6.0f / powf(dist, 2.0f), 3.0f / powf(dist, 3.0f),
                    0.0f, 0.0f, 3.0f / powf(dist, 2.0f), -3.0f / powf(dist, 3.0f),
                    0.0f, 0.0f, 0.0f, 1.0f / powf(dist, 3.0f)
            );
            auto powerToBernstein = glm::inverse(bernsteinToPower);

            glm::vec4 v0(bernsteinPoints[i*4 + 0].x, bernsteinPoints[i*4 + 1].x, bernsteinPoints[i*4 + 2].x, bernsteinPoints[i*4 + 3].x);
            glm::vec4 v1(bernsteinPoints[i*4 + 0].y, bernsteinPoints[i*4 + 1].y, bernsteinPoints[i*4 + 2].y, bernsteinPoints[i*4 + 3].y);
            glm::vec4 v2(bernsteinPoints[i*4 + 0].z, bernsteinPoints[i*4 + 1].z, bernsteinPoints[i*4 + 2].z, bernsteinPoints[i*4 + 3].z);

            v0 = powerToBernstein * v0;
            v1 = powerToBernstein * v1;
            v2 = powerToBernstein * v2;

            bernsteinPoints[i*4 + 0].x = v0.x;
            bernsteinPoints[i*4 + 1].x = v0.y;
            bernsteinPoints[i*4 + 2].x = v0.z;
            bernsteinPoints[i*4 + 3].x = v0.w;

            bernsteinPoints[i*4 + 0].y = v1.x;
            bernsteinPoints[i*4 + 1].y = v1.y;
            bernsteinPoints[i*4 + 2].y = v1.z;
            bernsteinPoints[i*4 + 3].y = v1.w;

            bernsteinPoints[i*4 + 0].z = v2.x;
            bernsteinPoints[i*4 + 1].z = v2.y;
            bernsteinPoints[i*4 + 2].z = v2.z;
            bernsteinPoints[i*4 + 3].z = v2.w;
        }
    }

    void addPoint(Point &newPoint) { // TODO Add reaction to the event of deleting a point from pointSet.
        controlPoints.emplace_back(newPoint.id, newPoint);
//        recalculateBernsteinPoints();
        updateMesh();
    }

    void removePoint(int i) {
        controlPoints.erase(controlPoints.begin() + i);
//        recalculateBernsteinPoints();
        updateMesh();
    }

    void updatePoint(Point &point, int i) {
//        recalculateBernsteinPoints();
        updateMesh();
    }

    void updateMesh() {
        std::vector<PositionVertex> vertices;
        vertices.reserve(bernsteinPoints.size());
        for(auto position : bernsteinPoints) {
            vertices.emplace_back(position);
        }
        std::vector<unsigned int> indices;
        indices.reserve(bernsteinPoints.size());
        for(int i = 0; i < bernsteinPoints.size(); i ++) {
            indices.push_back(i);
        }
        mesh.update(std::move(vertices), std::move(indices));
    }

    void render(Shader &shader) {
        glLineWidth(2);
        shader.setUniform("fullCurves", (int)(controlPoints.size()-1));
        shader.setUniform("remainder", 0);
        shader.setUniform("division", instanceCount);
        shader.setUniform("adaptationMultiplier", adaptationMultiplier);
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        mesh.render(instanceCount);
        glLineWidth(1);
    }
};

#endif //OPENGL_SANDBOX_INTERPOLATEDC2_H
