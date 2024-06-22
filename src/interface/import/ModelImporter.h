//
// Created by faliszewskii on 21.06.24.
//

#ifndef MATERIAL_PAINTER_MODELIMPORTER_H
#define MATERIAL_PAINTER_MODELIMPORTER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <memory>
#include "../../opengl/mesh/PosNorTexVertex.h"
#include "../../opengl/mesh/Mesh.h"
#include "../../model/material/Material.h"
#include "../../model/model/Model.h"

class FailedToLoadModelException : public std::runtime_error {
public:
    explicit FailedToLoadModelException(const std::string &msg) : std::runtime_error{msg} {};
};

class ModelImporter {
public:
    Model importModel(const std::string &resourcePath);

private:
    std::string directory;

    std::unique_ptr<Model::ModelNode> processNode(aiNode *node, const aiScene *scene, std::vector<std::unique_ptr<Material>> &materials);

    Model::ModelLeaf processMesh(aiMesh *mesh, std::vector<std::unique_ptr<Material>> &materials, unsigned int i);
    std::vector<std::unique_ptr<Material>> processMaterials(const aiScene *scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, std::vector<std::pair<std::string, Texture>> &loadedTextures, aiTextureType type, const std::string &typeName);

    void color4_to_vec4(aiColor4D *color, glm::vec4 p);

};


#endif //MATERIAL_PAINTER_MODELIMPORTER_H
