//
// Created by faliszewskii on 02.05.24.
//

#include "BezierPatchCreator.h"

void BezierPatchCreator::reset() {
    params = defaultParams;
    updatePreview();
}

void BezierPatchCreator::updatePreview() {

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
    addVertex(vertices, beginPos, eq, dance, 0, 0); // Corner
    for(int k = 0; k < 3 * params.patchCountWidth - params.wrapped?1:0; k++)
        addVertex(vertices, beginPos, eq, dance, 1 + k, 0);
    for(int l = 0; l < 3 * params.patchCountLength; l++) {
        for(int k = 0; k < 3 * params.patchCountWidth + 1 - params.wrapped?1:0; k++) {
            addVertex(vertices, beginPos, eq, dance, k, 1 + l);
        }
    }

    std::vector<unsigned int> indices;
    for(int n = 0; n < params.patchCountWidth; n++) {
        for(int m = 0; m < params.patchCountLength; m++) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    int k = i + 3 * n;
                    int l = j + 3 * m;
                    if(params.wrapped) {
                        if(n == params.patchCountWidth-1 && i == 3) {
                            indices.push_back(l * (3*params.patchCountWidth));
                        } else {
                            indices.push_back(k + l * (3*params.patchCountWidth));
                        }
                    } else {
                        indices.push_back(k + l * (3*params.patchCountWidth+1));
                    }
                }
            }
        }
    }

    auto points = vertices;
    previewMesh.update(std::move(vertices), std::move(indices));
    previewPoints.update(std::move(points), {});
}

void BezierPatchCreator::addVertex(std::vector<PositionVertex> &vertices, glm::vec3 beginPos,
                                   const std::function<glm::vec3(float, float)>& eq,
                                   const std::function<glm::vec3(float, float)>& dance,
                                   int k, int l) const {
    float time = glfwGetTime();
    auto pos = beginPos;
    pos += eq(k / 3.f / params.patchCountWidth, l / 3.f / params.patchCountLength);
    pos += dance(k / 3.f / params.patchCountWidth + time * 2.f, l / 3.f / params.patchCountWidth + time * 2.f);
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
    return previewMesh.getIndices().value();
}

std::vector<PositionVertex> BezierPatchCreator::getPointVertices() {
    params.dance = false;
    updatePreview();
    return previewPoints.getVertices();
}

std::vector<unsigned int> BezierPatchCreator::getGridIndices() const {
    std::vector<unsigned int> indices;
    int n = params.patchCountWidth * 3 + (params.wrapped?0:1);
    int m = params.patchCountLength * 3 + 1;
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
