//
// Created by faliszewskii on 02.05.24.
//

#include "BezierPatchCreator.h"

void BezierPatchCreator::reset() {
    params = defaultParams;
    updatePreview();
}

void BezierPatchCreator::updatePreview() {

    int vert_n = params.C2 ? params.patchCountWidth + 3 : 3 * params.patchCountWidth + 1;
    int wrappedOverlap = params.C2 ? 3 : 1;
    vert_n -= params.wrapped ? wrappedOverlap : 0;
    int vert_m = params.C2 ? params.patchCountLength + 3 : 3 * params.patchCountLength + 1;

    glm::vec3 beginPos;
    std::function<glm::vec3(float, float)> eq;
    std::function<glm::vec3(float, float)> dance;

    if(params.wrapped) {
        beginPos = {0, 0, -params.cylinderHeight / 2.f};
        eq = [&](float i, float j) {
            return glm::vec3(
                    params.cylinderRadius * cos(i * glm::two_pi<float>()),
                    params.cylinderRadius * sin(i * glm::two_pi<float>()),
                    j * params.cylinderHeight);
        };
        dance = [&](float x, float y) {
            return glm::vec3(0, 0.05 * sin(x) * cos(y), 0);
        };
    } else {
        beginPos = {-params.planeWidth / 2, 0, -params.planeLength / 2};
        eq = [&](float i, float j) { return glm::vec3(i * params.planeWidth, 0, j * params.planeLength); };
        dance = [&](float x, float y) {
            return glm::vec3(0, 0.05 * sin(x) * cos(y), 0);
        };
    }
    if(!params.dance) dance = [](float x, float y){return glm::vec3();};

    std::vector<PositionVertex> vertices;
    addVertex(vertices, beginPos, eq, dance, 0, 0, vert_n, vert_m); // Corner
    for(int k = 0; k < vert_n - 1; k++)
        addVertex(vertices, beginPos, eq, dance, 1 + k, 0, vert_n, vert_m);
    for(int l = 0; l < vert_m - 1; l++) {
        for(int k = 0; k < vert_n; k++) {
            addVertex(vertices, beginPos, eq, dance, k, 1 + l, vert_n, vert_m);
        }
    }

    std::vector<unsigned int> indices = getPatchIndices();

    auto points = vertices;
    previewMesh.update(std::move(vertices), std::move(indices));
    previewPoints.update(std::move(points), {});
}

void BezierPatchCreator::addVertex(std::vector<PositionVertex> &vertices, glm::vec3 beginPos,
                                   const std::function<glm::vec3(float, float)>& eq,
                                   const std::function<glm::vec3(float, float)>& dance,
                                   int k, int l, float width, float length) const {
    int w = params.wrapped ? 0 : 1;
    float time = glfwGetTime();
    auto pos = beginPos;
    pos += eq(k / (width-w), l / (length-w));
    pos += dance(k / (width-w) + time * 2.f, l / (width-w) + time * 2.f);
    vertices.emplace_back(pos);
}

void BezierPatchCreator::renderPreview(Shader &shader) {
    glLineWidth(2);
    if(params.dance) updatePreview();
    glPatchParameteri(GL_PATCH_VERTICES, 16);
    previewMesh.render(2);
    glLineWidth(1);
}

void BezierPatchCreator::renderPoints(Shader &shader) const {
    shader.setUniform("position", glm::vec3());
    shader.setUniform("selected", false);
    shader.setUniform("color", glm::vec4(0,0,0,1));
    glPointSize(6);
    previewPoints.render();
    glPointSize(1);
}

BezierPatchCreatorParams BezierPatchCreator::getParams() {
    return params;
}

void BezierPatchCreator::update(BezierPatchCreatorParams newParams) {
    params = newParams;
    updatePreview();
}

std::vector<PositionVertex> BezierPatchCreator::getPatchVertices() {
    params.dance = false;
    updatePreview();
    return previewMesh.getVertices();
}

std::vector<unsigned int> BezierPatchCreator::getPatchIndices() {
    int vert_n = params.C2 ? params.patchCountWidth + 3 : 3 * params.patchCountWidth + 1;
    int wrappedOverlap = params.C2 ? 3 : 1;
    vert_n -= params.wrapped ? wrappedOverlap : 0;

    int step = params.C2 ? 1 : 3;
    std::vector<unsigned int> indices;
    for(int m = 0; m < params.patchCountLength; m++) {
    for(int n = 0; n < params.patchCountWidth; n++) {
            for (int j = 0; j < 4; j++) {
                for (int i = 0; i < 4; i++) {
                    int k = i + step * n;
                    int l = j + step * m;
//                    int t = n - params.patchCountWidth + wrappedOverlap - 3 + i;
                    if(params.wrapped && k >= vert_n) {
                        indices.push_back(l * vert_n + (k - vert_n));
                    } else {
                        indices.push_back(k + l * vert_n);
                    }
                }
            }
        }
    }
    return indices;
}

std::vector<PositionVertex> BezierPatchCreator::getPointVertices() {
    params.dance = false;
    updatePreview();
    return previewPoints.getVertices();
}

std::vector<unsigned int> BezierPatchCreator::getGridIndices() const {
    std::vector<unsigned int> indices;
    int n = params.C2 ? params.patchCountWidth + 3 : 3 * params.patchCountWidth + 1;
    n -= params.wrapped?1:0;
    int m = params.C2 ? params.patchCountLength + 3 : 3 * params.patchCountLength + 1;
    for(int k = 0; k < n; k++) {
        for(int l = 0; l < m; l++) {
            if(k != n - 1) {
                indices.push_back(n * l + k);
                indices.push_back(n * l + (k + 1));
            } else if(params.wrapped) {
                indices.push_back(n * l + k);
                indices.push_back(n * l);
            }

            if(l != m - 1) {
                indices.push_back(n * l + k);
                indices.push_back(n * (l + 1) + k);
            }
        }
    }
    return indices;
}
