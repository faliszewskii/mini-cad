//
// Created by faliszewskii on 28.12.23.
//

#ifndef OPENGL_SANDBOX_ASSETIMPORTER_H
#define OPENGL_SANDBOX_ASSETIMPORTER_H


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include "ImportResult.h"

class FailedToLoadModelException : public std::runtime_error {
public:
    explicit FailedToLoadModelException(const std::string &msg) : std::runtime_error{msg} {};
};

class AssetImporter {
public:
    ImportResult importModel(const std::string &resourcePath);

private:
    std::string directory;

    std::unique_ptr<TransformTree> processNode(aiNode *node, const aiScene *scene, std::vector<std::unique_ptr<Material>> &materials);

    Mesh<Vertex> processMesh(aiMesh *mesh, std::vector<std::unique_ptr<Material>> &materials, unsigned int i);
    std::vector<std::unique_ptr<Material>> processMaterials(const aiScene *scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, std::vector<Texture> &loadedTextures, aiTextureType type, const std::string &typeName);

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
