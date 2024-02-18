//
// Created by faliszewskii on 28.12.23.
//

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <iostream>

#include "AssetImporter.h"
#include "../io/stb_image.h"
#include "../../presentation/scene/nodes/transformation/Transformation.h"

ImportResult AssetImporter::importModel(const std::string &resourcePath) {
    Assimp::Importer import;
    import.SetPropertyBool(AI_CONFIG_FBX_CONVERT_TO_M, true);

    const aiScene *scene = import.ReadFile(resourcePath,
                                           aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_ValidateDataStructure);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        throw FailedToLoadModelException("Failed to load: " + resourcePath);
    }
    directory = resourcePath.substr(0, resourcePath.find_last_of('/'));

    auto materials = processMaterials(scene);
    auto transformTree = processNode(scene->mRootNode, scene, materials);
    return ImportResult(std::move(transformTree), std::move(materials));
}

std::unique_ptr<TransformTree> AssetImporter::processNode(aiNode *node, const aiScene *scene, std::vector<std::unique_ptr<Material>> &materials) {
    std::string name = node->mName.C_Str();

    aiMatrix4x4t<ai_real> t = node->mTransformation;
    glm::mat4 transformation(
            t.a1, t.b1, t.c1, t.d1,
            t.a2, t.b2, t.c2, t.d2,
            t.a3, t.b3, t.c3, t.d3,
            t.a4, t.b4, t.c4, t.d4
    );


    auto transformTree = std::make_unique<TransformTree>(TransformTree(name, transformation));

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        auto child = processNode(node->mChildren[i], scene, materials);
        transformTree->addChlid(std::move(child));
    }

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        auto mesh = std::make_unique<Mesh>(processMesh(scene->mMeshes[node->mMeshes[i]], materials, i));
        transformTree->addChlid(std::move(mesh));
    }

    return transformTree;
}

Mesh AssetImporter::processMesh(aiMesh *mesh, std::vector<std::unique_ptr<Material>> &materials, unsigned int index) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    aiString meshName = mesh->mName;
    if (meshName == aiString())
        meshName = std::string("mesh.") + std::to_string(index);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
//            // tangent
//            vector.x = mesh->mTangents[i].x;
//            vector.y = mesh->mTangents[i].y;
//            vector.z = mesh->mTangents[i].z;
//            vertex.tangent = vector;
//            // bitangent
//            vector.x = mesh->mBitangents[i].x;
//            vector.y = mesh->mBitangents[i].y;
//            vector.z = mesh->mBitangents[i].z;
//            vertex.bitangent = vector;
        } else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // TODO global table of materials and SceneTreeNode only has reference to SceneNodes.


    return {meshName.C_Str(), vertices, indices, *materials[mesh->mMaterialIndex]};
}

std::vector<std::unique_ptr<Material>> AssetImporter::processMaterials(const aiScene *scene) {
    std::vector<std::unique_ptr<Material>> materials;
    std::vector<Texture> loadedTextures;
    for(int i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial *material = scene->mMaterials[i];

        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, loadedTextures, aiTextureType_DIFFUSE, "texture_diffuse");
        // TODO Multiple textures
//    // 2. specular maps
//    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//    // 3. normal maps
//    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
//    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
//    // 4. height maps
//    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
//    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        glm::vec4 diffuse(0.8f, 0.8f, 0.8f, 1.0f);
        C_STRUCT aiColor4D _diffuse;
        if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &_diffuse))
            color4_to_vec4(&_diffuse, diffuse);
        aiString materialName;
        if (AI_SUCCESS != material->Get(AI_MATKEY_NAME, materialName))
            materialName = std::string("material.") + std::to_string(i);
        aiShadingMode shadingMode = aiShadingMode_Gouraud;
        if (AI_SUCCESS != material->Get(AI_MATKEY_SHADING_MODEL, shadingMode)) {}
        float shininess = 0;
        if (AI_SUCCESS != material->Get(AI_MATKEY_SHININESS, shininess)) {}

        // TODO Multiple textures
        materials.push_back(std::make_unique<Material>(materialName.C_Str(), diffuse,
                                                       !diffuseMaps.empty() ? diffuseMaps[0] : std::optional<Texture>(), shininess, shadingModelMap[shadingMode]));
    }
    return materials;
}

std::vector<Texture> AssetImporter::loadMaterialTextures(aiMaterial *mat, std::vector<Texture> &loadedTextures, aiTextureType type, const std::string &typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < loadedTextures.size(); j++) {
            if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(loadedTextures[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip) {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = textureFromFile(str.C_Str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }
    return textures;
}

unsigned int AssetImporter::textureFromFile(const char *path) {
    std::string filename = std::string(path);
    std::replace(filename.begin(), filename.end(), '\\', '/');
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void AssetImporter::color4_to_vec4(aiColor4D *color, glm::vec4 p) {
    p[0] = color->r;
    p[1] = color->g;
    p[2] = color->b;
    p[3] = color->a;
}
