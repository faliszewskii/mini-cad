//
// Created by faliszewskii on 06.05.24.
//

#ifndef OPENGL_SANDBOX_PATCHC2_H
#define OPENGL_SANDBOX_PATCHC2_H

#include <string>
#include "../vertices/PositionVertex.h"
#include "../ids/IdCounter.h"
#include "../entities/mesh/Mesh.h"

class PatchC2 {
    Mesh<PositionVertex> meshGrid;
    Texture mask;
    std::array<std::array<glm::vec3, 256>, 256> maskData;
public:
    int id;
    std::string name;
    bool selected;

    int bezierPatchGridWidth = 3;
    int bezierPatchGridLength = 3;
    int patchCountX;
    int patchCountY;
    bool wrapped;
    bool drawBezierGrid = false;
    std::vector<std::pair<int, std::reference_wrapper<Point>>> controlPoints;

    PatchC2(const std::vector<PositionVertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<unsigned int> &gridIndices) :
        id(IdCounter::nextId()), name("Patch C2 ("+std::to_string(id)+")"), selected(false),
        mesh(vertices, indices,GL_PATCHES),
    meshGrid(vertices, gridIndices,GL_LINES),
    mask(256, 256, 1, GL_SRGB_ALPHA, GL_RGBA, GL_FLOAT, GL_TEXTURE_2D) {
        for(auto &row : maskData)
            for(auto &el : row)
                el = glm::vec3(0);
    }

    void updatePoint(Point &point, int i) {
        mesh.update({point.position}, i);
        meshGrid.update({point.position}, i);
    }

    void render(Shader &shader) {
        glLineWidth(2);
        shader.setUniform("gridCountWidth", bezierPatchGridWidth);
        shader.setUniform("gridCountLength", bezierPatchGridLength);
        shader.setUniform("patchCountWidth", patchCountX);
        shader.setUniform("patchCountLength", patchCountY);
        shader.setUniform("selected", selected);
        mask.bind(0);
        shader.setUniform("mask", 0);
        glPatchParameteri(GL_PATCH_VERTICES, 16);
        mesh.render(2);
        glLineWidth(1);
    }

    void renderBezierGrid(Shader &shader) {
        if(!drawBezierGrid) return;
        shader.setUniform("selected", false);
        shader.setUniform("model", glm::mat4(1.0f));
        meshGrid.render();
    }


    float rangeU() const { return patchCountX; }
    float rangeV() const { return patchCountY; }
    bool wrapU() const { return wrapped; }
    bool wrapV() const { return false; }

    glm::vec4 DeBoorCoeffs(float t, int i, int knotCount)
    {
        float knotDist = 1.0f / knotCount;

        float N[5];
        float A[4];
        float B[4];

        N[1] = 1;
        for (int j = 1; j <= 3; j++)
        {
            A[j] = knotDist * (i + j) - t;
            B[j] = t - knotDist * (i + 1 - j);
            float saved = 0;
            for (int k = 1; k <= j; k++)
            {
                float term = N[k] / (A[k] + B[j + 1 - k]);
                N[k] = saved + A[k] * term;
                saved = B[j + 1 - k] * term;
            }
            N[j + 1] = saved;
        }

        return { N[1], N[2], N[3], N[4] };
    }

    glm::vec4 DeBoorDerivs(float t, int i, int knotCount)
    {
        float knotDist = 1.0f / knotCount;

        float N[5];
        float dN[5];
        float A[4];
        float B[4];

        N[1] = 1;
        for (int j = 1; j <= 3; j++) {
            float saved = 0.0f;
            for (int k = 1; k <= j; k++) {
                float term = N[k] / (A[k] + B[j + 1 - k]);
                float derivativeTerm = (N[k + 1] - N[k]) / (A[k] + B[j + 1 - k]);
                N[k] = saved + A[k] * term;
                saved = B[j + 1 - k] * term;
                dN[j] += derivativeTerm;
            }
            dN[j + 1] = saved;
        }

        return { dN[1], dN[2], dN[3], dN[4]};
    }


    void convertToSinglePatch(float &u, float &v, glm::vec3 coefficients[][4]) {
        auto indices = mesh.getIndices().value();
        int i = static_cast<int>(u);
        int j = static_cast<int>(v);
        if(i == patchCountX) i--;
        if(j == patchCountY) j--;
        for(int k=0; k<16; k++) {
            unsigned int id = indices[ 16*(j*patchCountX + i) + k];
            glm::vec3 point = controlPoints[id].second.get().position;
            coefficients[k%4][k/4] = point;
        }
        u = u - i;
        v = v - j;
    }

    glm::vec3 evaluate (float u, float v)
    {
        glm::vec3 p[4][4];
        convertToSinglePatch(u, v, p);

        int idxU = patchCountX + 3;
        int idxV = patchCountY + 3;
        int countU = 3 * this->rangeU() + 4;
        int countV = 3 * this->rangeV() + 4;

        float u0 = float(idxU) / countU;
        float u1 = float(idxU + 1) / countU;
        float v0 = float(idxV) / countV;
        float v1 = float(idxV + 1) / countV;

        u = u0 + (u1 - u0) * u;
        v = v0 + (v1 - v0) * v;

        glm::vec4 uN = DeBoorCoeffs(u, idxU, countU);
        glm::vec4 vN = DeBoorCoeffs(v, idxV, countV);

        glm::vec3 point = uN[0] * ( vN[0]*p[0][0] + vN[1]*p[0][1] + vN[2]*p[0][2] + vN[3]*p[0][3] )
                        + uN[1] * ( vN[0]*p[1][0] + vN[1]*p[1][1] + vN[2]*p[1][2] + vN[3]*p[1][3] )
                        + uN[2] * ( vN[0]*p[2][0] + vN[1]*p[2][1] + vN[2]*p[2][2] + vN[3]*p[2][3] )
                        + uN[3] * ( vN[0]*p[3][0] + vN[1]*p[3][1] + vN[2]*p[3][2] + vN[3]*p[3][3] );

        return point;

    }

    glm::vec3 evaluateDU (float u, float v)
    {
        float h = 0.001f;

        glm::vec3 p1 = evaluate(u, v);
        glm::vec3 p2 = evaluate(u + h, v);

        return (p2 - p1) / h;
    }

    glm::vec3 evaluateDV (float u, float v)
    {
        float h = 0.001f;

        glm::vec3 p1 = evaluate(u, v);
        glm::vec3 p2 = evaluate(u, v + h);

        return (p2 - p1) / h;
    }


    [[nodiscard]] std::pair<std::reference_wrapper<Texture>, std::reference_wrapper<std::array<std::array<glm::vec3, 256>, 256>>> getMask() {
        return std::make_pair(std::ref(mask), std::ref(maskData));
    }

    void setMask(std::array<std::array<glm::vec3, 256>, 256> newMask) {
        maskData = newMask;
        std::vector<float> flattenedArray;
        flattenedArray.reserve(256 * 256 * 4);
        for (const auto& row : maskData) {
            for (const auto& vec : row) {
                flattenedArray.push_back(vec.x);
                flattenedArray.push_back(vec.y);
                flattenedArray.push_back(vec.z);
                flattenedArray.push_back(1);
            }
        }
        mask.update2D(flattenedArray.data());
    }

    Mesh<PositionVertex> mesh;
};

#endif //OPENGL_SANDBOX_PATCHC2_H
