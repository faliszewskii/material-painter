//
// Created by faliszewskii on 22.06.24.
//

#include "ModelRenderer.h"

void ModelRenderer::render(Model &model, Shader &shader) {
    render(*model.root, shader, glm::identity<glm::mat4>());
}

void ModelRenderer::render(Model::ModelNode &node, Shader &shader, glm::mat4 transform) {
    transform *= node.transformation.getTransform();
    shader.setUniform("model", transform);
    for(auto &leaf : node.leafs) {
        shader.setUniform("material.hasTexture", leaf->material.getDiffuseTexture().has_value());
        shader.setUniform("material.texture", 0);
        if(leaf->material.getDiffuseTexture()) leaf->material.getDiffuseTexture()->bind(0);
        shader.setUniform("material.albedo", leaf->material.getAlbedo());
        shader.setUniform("material.shininess", leaf->material.getShininess());
        leaf->mesh.render();
    }
    for(auto &child : node.children) {
        render(*child, shader, transform);
    }
}
