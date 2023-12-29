//
// Created by faliszewskii on 28.12.23.
//

#ifndef OPENGL_SANDBOX_ASSETIMPORTER_H
#define OPENGL_SANDBOX_ASSETIMPORTER_H


#include "../../presentation/rendering/model/Model.h"
#include "../../presentation/rendering/model/Mesh.h"

class AssetImporter {
public:
    int importModel(std::string resourcePath, Model** result);
private:
    std::vector<Texture> textures_loaded;
    std::string directory;

    ModelNode processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName);

    unsigned int textureFromFile(const char *path, const std::string &directory);
};


#endif //OPENGL_SANDBOX_ASSETIMPORTER_H
