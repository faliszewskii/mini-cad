//
// Created by faliszewskii on 28.12.23.
//

#ifndef OPENGL_SANDBOX_ASSETIMPORTER_H
#define OPENGL_SANDBOX_ASSETIMPORTER_H


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../presentation/rendering/model/Mesh.h"
#include "../../presentation/rendering/model/ModelNode.h"

class AssetImporter {
public:
    std::optional<ModelNode> importModel(std::string resourcePath);
private:
    std::vector<Texture> textures_loaded;
    std::string directory;

    ModelNode processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName);

    unsigned int textureFromFile(const char *path);

    void color4_to_vec4(aiColor4D *color, glm::vec4 p);
};


#endif //OPENGL_SANDBOX_ASSETIMPORTER_H
