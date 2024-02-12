//
// Created by faliszewskii on 28.12.23.
//

#ifndef OPENGL_SANDBOX_ASSETIMPORTER_H
#define OPENGL_SANDBOX_ASSETIMPORTER_H


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include "../../presentation/scene/nodes/mesh/Mesh.h"
#include "../Scene.h"

class FailedToLoadModelException : public std::runtime_error {
public:
    explicit FailedToLoadModelException(const std::string &msg) : std::runtime_error{msg} {};
};

class AssetImporter {
public:
    std::unique_ptr<Scene> importModel(const std::string &resourcePath);

private:
    std::vector<Texture> textures_loaded;
    std::string directory;

    std::unique_ptr<Scene> processNode(aiNode *node, const aiScene *scene);

    std::unique_ptr<Scene> processMesh(aiMesh *mesh, const aiScene *scene, unsigned int i);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);

    unsigned int textureFromFile(const char *path);

    void color4_to_vec4(aiColor4D *color, glm::vec4 p);


    std::map<aiShadingMode, std::string> shadingModelMap{
            {aiShadingMode_Flat,         "Flat"},
            {aiShadingMode_Gouraud,      "Gouraud"},
            {aiShadingMode_Phong,        "Phong"},
            {aiShadingMode_Blinn,        "Blinn-Phong"},
            {aiShadingMode_Toon,         "Toon"},
            {aiShadingMode_OrenNayar,    "Oren-Nayar"},
            {aiShadingMode_Minnaert,     "Minnaert"},
            {aiShadingMode_CookTorrance, "Cook-Torrance"},
            {aiShadingMode_NoShading,    "No Shading"},
            {aiShadingMode_Fresnel,      "Fresnel"},
            {aiShadingMode_PBR_BRDF,     "PBR BRDF"}
    };
};


#endif //OPENGL_SANDBOX_ASSETIMPORTER_H
