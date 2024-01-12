//
// Created by faliszewskii on 28.12.23.
//

#ifndef OPENGL_SANDBOX_ASSETIMPORTER_H
#define OPENGL_SANDBOX_ASSETIMPORTER_H


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../presentation/scene/tree/SceneTreeNode.h"
#include "../../presentation/scene/nodes/mesh/Mesh.h"

class AssetImporter {
public:
    std::optional<std::unique_ptr<SceneTreeNode>> importModel(const std::string& resourcePath);
private:
    std::vector<Texture> textures_loaded;
    std::string directory;

    std::unique_ptr<SceneTreeNode> processNode(aiNode *node, const aiScene *scene);
    std::unique_ptr<SceneTreeNode> processMesh(aiMesh *mesh, const aiScene *scene, unsigned int i);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName);

    unsigned int textureFromFile(const char *path);

    void color4_to_vec4(aiColor4D *color, glm::vec4 p);
};


#endif //OPENGL_SANDBOX_ASSETIMPORTER_H
