//
// Created by faliszewskii on 21.06.24.
//

#include "Model.h"

#include <utility>

Model::ModelLeaf::ModelLeaf(std::vector<PosNorTexVertex> vertices, std::vector<unsigned int> indices, Mesh<PosNorTexVertex> mesh, Material &material) :
vertices(std::move(vertices)), indices(std::move(indices)), mesh(mesh), material(material) {}

Model::ModelNode::ModelNode(Transformation transformation) : transformation(transformation) {}

Model::ModelNode::ModelNode() = default;

Model::Model(std::unique_ptr<ModelNode> &&root, std::vector<std::unique_ptr<Material>> materials) :
    root(std::forward<std::unique_ptr<ModelNode>>(root)), materials(std::move(materials)) {

}
