//
// Created by faliszewskii on 02.05.24.
//

#ifndef OPENGL_SANDBOX_BEZIERPATCHCREATOR_H
#define OPENGL_SANDBOX_BEZIERPATCHCREATOR_H


#include "../entities/mesh/Mesh.h"
#include "../vertices/PositionVertex.h"
#include "BezierPatchCreatorParams.h"
#include "../geometry/PatchC0.h"
#include "../geometry/PatchC2.h"

class BezierPatchCreator {
    Mesh<PositionVertex> previewMesh;
    Mesh<PositionVertex>  previewPoints;

    BezierPatchCreatorParams params;

    constexpr static BezierPatchCreatorParams defaultParams{
            1,
            1,
            1,
            1,
            1,
            0.5f,
            false,
            false,
            true
    };

public:
    BezierPatchCreator() :
    previewMesh({}, {}, GL_PATCHES),
    previewPoints({}, {}, GL_POINTS) ,
    params(defaultParams) {
        updatePreview();
    }

    void reset();
    void update(BezierPatchCreatorParams newParams);

    BezierPatchCreatorParams getParams();

    void updatePreview();
    void renderPreview(Shader &shader);
    void renderPoints(Shader &shader) const;


    void addVertex(std::vector<PositionVertex> &vertices, glm::vec3 beginPos,
                   const std::function<glm::vec3(float, float)>& eq,
                   const std::function<glm::vec3(float, float)>& dance,
                   int k, int l) const;


    std::vector<PositionVertex> getPatchVertices();
    std::vector<PositionVertex> getPointVertices();
    std::vector<unsigned int> getPatchIndices();
    std::vector<unsigned int> getGridIndices() const;

};


#endif //OPENGL_SANDBOX_BEZIERPATCHCREATOR_H
