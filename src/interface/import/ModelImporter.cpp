//
// Created by faliszewskii on 21.06.24.
//

#include <glm/ext/matrix_float4x4.hpp>
#include "ModelImporter.h"


Model ModelImporter::importModel(const std::string &resourcePath) {
    Assimp::Importer import;
    import.SetPropertyBool(AI_CONFIG_FBX_CONVERT_TO_M, true);

    const aiScene *scene = import.ReadFile(resourcePath,
                                           aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_ValidateDataStructure);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw FailedToLoadModelException("Failed to load: " + resourcePath);
    }
    directory = resourcePath.substr(0, resourcePath.find_last_of('/'));

    auto materials = processMaterials(scene);
    auto root = processNode(scene->mRootNode, scene, materials);
    return Model(std::move(root), std::move(materials));
}

std::unique_ptr<Model::ModelNode> ModelImporter::processNode(aiNode *node, const aiScene *scene, std::vector<std::unique_ptr<Material>> &materials) {
    std::string name = node->mName.C_Str();

    aiMatrix4x4t<ai_real> t = node->mTransformation;
    glm::mat4 transformation(
            t.a1, t.b1, t.c1, t.d1,
            t.a2, t.b2, t.c2, t.d2,
            t.a3, t.b3, t.c3, t.d3,
            t.a4, t.b4, t.c4, t.d4
    );

    auto modelNode = std::make_unique<Model::ModelNode>(Transformation(transformation));

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        auto child = processNode(node->mChildren[i], scene, materials);
        modelNode->children.push_back(std::move(child));
    }

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        auto mesh = std::make_unique<Model::ModelLeaf>(processMesh(scene->mMeshes[node->mMeshes[i]], materials, i));
        modelNode->leafs.push_back(std::move(mesh));

    }

    return modelNode;
}

Model::ModelLeaf ModelImporter::processMesh(aiMesh *mesh, std::vector<std::unique_ptr<Material>> &materials, unsigned int index) {
    std::vector<PosNorTexVertex> vertices;
    std::vector<unsigned int> indices;

    aiString meshName = mesh->mName;
    if (meshName == aiString())
        meshName = std::string("mesh.") + std::to_string(index);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        PosNorTexVertex vertex{};
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
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        } else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return {vertices, indices, Mesh<PosNorTexVertex>(vertices, indices), *materials[mesh->mMaterialIndex]};
}

std::vector<std::unique_ptr<Material>> ModelImporter::processMaterials(const aiScene *scene) {
    std::vector<std::unique_ptr<Material>> materials;
    std::vector<std::pair<std::string, Texture>> loadedTextures;
    for(int i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial *material = scene->mMaterials[i];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, loadedTextures, aiTextureType_DIFFUSE, "texture_diffuse");
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

        materials.push_back(std::make_unique<Material>(diffuse, !diffuseMaps.empty() ? diffuseMaps[0] : std::optional<Texture>(), shininess));
    }
    return materials;
}

std::vector<Texture> ModelImporter::loadMaterialTextures(aiMaterial *mat, std::vector<std::pair<std::string, Texture>> &loadedTextures, aiTextureType type, const std::string &typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < loadedTextures.size(); j++) {
            if (std::strcmp(loadedTextures[j].first.data(), str.C_Str()) == 0) {
                textures.push_back(loadedTextures[j].second);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip) {   // if texture hasn't been loaded already, load it
            Texture texture(str.C_Str(), true);
            textures.emplace_back(texture);
            loadedTextures.emplace_back(str.C_Str(), texture);
        }
    }
    return textures;
}

void ModelImporter::color4_to_vec4(aiColor4D *color, glm::vec4 p) {
    p[0] = color->r;
    p[1] = color->g;
    p[2] = color->b;
    p[3] = color->a;
}
