//
// Created by faliszewskii on 21.06.24.
//

#ifndef MATERIAL_PAINTER_MODEL_H
#define MATERIAL_PAINTER_MODEL_H


#include <vector>
#include <memory>
#include "../../opengl/mesh/PosNorTexVertex.h"
#include "../../opengl/mesh/Mesh.h"
#include "../material/Material.h"
#include "../transformation/Transformation.h"

struct Model {

public:

    struct ModelLeaf {
        std::vector<PosNorTexVertex> vertices;
        std::vector<unsigned int> indices;
        Mesh<PosNorTexVertex> mesh;
        Material &material;

        ModelLeaf(std::vector<PosNorTexVertex> vertices, std::vector<unsigned int> indices, Mesh<PosNorTexVertex> mesh, Material &material);
    };
    struct ModelNode {
        Transformation transformation;
        std::vector<std::unique_ptr<ModelNode>> children;
        std::vector<std::unique_ptr<ModelLeaf>> leafs;

        explicit ModelNode(Transformation transformation);
        explicit ModelNode();
    };

    std::unique_ptr<ModelNode> root;
    std::vector<std::unique_ptr<Material>> materials;

    Model() : root(std::make_unique<ModelNode>()) {}
    Model(std::unique_ptr<ModelNode> &&root, std::vector<std::unique_ptr<Material>> materials);
};


#endif //MATERIAL_PAINTER_MODEL_H
