//
// Created by faliszewskii on 06.05.24.
//

#ifndef OPENGL_SANDBOX_PATCHC0_H
#define OPENGL_SANDBOX_PATCHC0_H

#include <string>
#include "Point.h"
#include "../ids/IdCounter.h"
#include "../vertices/PositionVertex.h"
#include "../texture/Texture.h"

class PatchC0 {
    Mesh<PositionVertex> meshGrid;
    Texture mask;
    std::array<std::array<glm::vec3, 256>, 256> maskData;
public:
    Mesh<PositionVertex> mesh;

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

    PatchC0(const std::vector<PositionVertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<unsigned int> &gridIndices) :
        id(IdCounter::nextId()), name("Patch C0 ("+std::to_string(id)+")"), selected(false),
        mesh(vertices,indices,GL_PATCHES),
        meshGrid(vertices, gridIndices,GL_LINES),
        mask(256, 256, 4, GL_SRGB_ALPHA, GL_RGBA, GL_FLOAT, GL_TEXTURE_2D) {
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

    void convertToSinglePatch(float &u, float &v, glm::vec3 coefficients[][4]) const {
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

    glm::vec4 descendingAlgorithm(float t, int n) const
    {
        glm::mat4 bernsteinBasis = glm::mat4(0.0f);
        bernsteinBasis[0][0] = 1.0f;

        float u = 1.0 - t;

        for (int j = 1; j <= n; j++)
        {
            bernsteinBasis[j][0] = bernsteinBasis[j - 1][0] * u;

            for (int i = 1; i <= j; i++)
            {
                bernsteinBasis[j][i] = bernsteinBasis[j - 1][i] * u + bernsteinBasis[j - 1][i - 1] * t;
            }
        }

        return glm::vec4(bernsteinBasis[n][0], bernsteinBasis[n][1], bernsteinBasis[n][2], bernsteinBasis[n][3]);
    }


    glm::vec3 deCasteljau2D(glm::vec3 coefficients[4][4], float u, float v) const
    {
        glm::vec4 uCoeffs = descendingAlgorithm(u, 3);
        glm::vec4 vCoeffs = descendingAlgorithm(v, 3);

        glm::vec3 result = glm::vec3(0.0, 0.0, 0.0);

        for (int u = 0; u < 4; u++)
        {
            glm::vec3 partResult = glm::vec3(0.0, 0.0, 0.0);

            partResult += coefficients[u][0] * vCoeffs[0];
            partResult += coefficients[u][1] * vCoeffs[1];
            partResult += coefficients[u][2] * vCoeffs[2];
            partResult += coefficients[u][3] * vCoeffs[3];

            result += partResult * uCoeffs[u];
        }

        return result;
    }

    void cap(float &point, float lowerLimit, float upperLimit, bool wrap) const {
        if(wrap) {
            while(point < lowerLimit) point += upperLimit - lowerLimit;
            while(point > upperLimit) point -= upperLimit - lowerLimit;
        } else {
            if(point < lowerLimit) point = lowerLimit;
            if(point > upperLimit) point = upperLimit;
        }
    }

    [[nodiscard]] glm::vec3 evaluate(float u, float v) const {
        // TODO take care of multipatches
        glm::vec3 coefficients[4][4];
        convertToSinglePatch(u, v, coefficients);

        return deCasteljau2D(coefficients, u, v);
    }

    [[nodiscard]] glm::vec3 evaluateDU(float u, float v) const {
        glm::vec3 coefficients[4][4];
        convertToSinglePatch(u, v, coefficients);
        float dbu[4] = { -3.0f*(1.0f-u)*(1.0f-u), 3.0f*(1.0f-u)*(1.0f-3.0f*u), 3.0f*u*(2.0f-3.0f*u), 3.0f*u*u };
        float bv[4] = { (1.0f-v)*(1.0f-v)*(1.0f-v), 3.0f*v*(1.0f-v)*(1.0f-v), 3.0f*v*v*(1.0f-v), v*v*v };

        // Tablica punktów kontrolnych w postaci 1D
        glm::vec3 uCoeffs3 = descendingAlgorithm(u, 2);
        glm::vec4 vCoeffs4 = descendingAlgorithm(v, 3);

        glm::vec3 partResult[4];
        glm::vec3 tangent1 = glm::vec3(0);
        for (int k = 0; k < 4; k++) {
            partResult[k] = glm::vec3(0.0, 0.0, 0.0);
            partResult[k] += coefficients[k][0] * vCoeffs4[0];
            partResult[k] += coefficients[k][1] * vCoeffs4[1];
            partResult[k] += coefficients[k][2] * vCoeffs4[2];
            partResult[k] += coefficients[k][3] * vCoeffs4[3];
        }
        tangent1 += (partResult[1] - partResult[0]) * uCoeffs3[0];
        tangent1 += (partResult[2] - partResult[1]) * uCoeffs3[1];
        tangent1 += (partResult[3] - partResult[2]) * uCoeffs3[2];
        tangent1 *= 3;

        return tangent1;
        // Wylicz współczynniki Bernsteina
        // Oblicz pozycję punktu na powierzchni
        glm::vec3 pos(0.0f);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                pos += dbu[i] * bv[j] * coefficients[i][j];
            }
        }

        return pos;
    }

    [[nodiscard]] glm::vec3 evaluateDV(float u, float v) const {
        // glm::vec3 coefficients[4][4] = {
        //     {controlPoints[0].second.get().position, controlPoints[4].second.get().position, controlPoints[8].second.get().position, controlPoints[12].second.get().position},
        //     {controlPoints[1].second.get().position, controlPoints[5].second.get().position, controlPoints[9].second.get().position, controlPoints[13].second.get().position},
        //     {controlPoints[2].second.get().position, controlPoints[6].second.get().position, controlPoints[10].second.get().position, controlPoints[14].second.get().position},
        //     {controlPoints[3].second.get().position, controlPoints[7].second.get().position, controlPoints[11].second.get().position, controlPoints[15].second.get().position}
        // };
        glm::vec3 coefficients[4][4]; /*= {
            {controlPoints[0].second.get().position, controlPoints[1].second.get().position, controlPoints[2].second.get().position, controlPoints[3].second.get().position},
            {controlPoints[4].second.get().position, controlPoints[5].second.get().position, controlPoints[6].second.get().position, controlPoints[7].second.get().position},
            {controlPoints[8].second.get().position, controlPoints[9].second.get().position, controlPoints[10].second.get().position, controlPoints[11].second.get().position},
            {controlPoints[12].second.get().position, controlPoints[13].second.get().position, controlPoints[14].second.get().position, controlPoints[15].second.get().position}
        };*/
        convertToSinglePatch(u, v, coefficients);
        glm::vec3 vCoeffs3 = descendingAlgorithm(v, 2);
        glm::vec4 uCoeffs4 = descendingAlgorithm(u, 3);

        glm::vec3 partResult[4];
        glm::vec3 tangent2 = glm::vec3(0);
        for (int k = 0; k < 4; k++) {
            partResult[k] = glm::vec3(0.0, 0.0, 0.0);
            partResult[k] += coefficients[0][k] * uCoeffs4[0];
            partResult[k] += coefficients[1][k] * uCoeffs4[1];
            partResult[k] += coefficients[2][k] * uCoeffs4[2];
            partResult[k] += coefficients[3][k] * uCoeffs4[3];
        }
        tangent2 += (partResult[1] - partResult[0]) * vCoeffs3[0];
        tangent2 += (partResult[2] - partResult[1]) * vCoeffs3[1];
        tangent2 += (partResult[3] - partResult[2]) * vCoeffs3[2];
        tangent2 *= 3;

        return tangent2;

        // Wylicz współczynniki Bernsteina
        float bu[4] = { (1.0f-u)*(1.0f-u)*(1.0f-u), 3.0f*u*(1.0f-u)*(1.0f-u), 3.0f*u*u*(1.0f-u), u*u*u };
        float dbv[4] = { -3.0f*(1.0f-v)*(1.0f-v), 3.0f*(1.0f-v)*(1.0f-3.0f*v), 3.0f*v*(2.0f-3.0f*v), 3.0f*v*v };

        // Oblicz pozycję punktu na powierzchni
        glm::vec3 pos(0.0f);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                pos += bu[i] * dbv[j] * coefficients[i][j];
            }
        }

        return pos;
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
};

#endif //OPENGL_SANDBOX_PATCHC0_H
