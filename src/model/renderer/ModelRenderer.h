//
// Created by faliszewskii on 22.06.24.
//

#ifndef MATERIAL_PAINTER_MODELRENDERER_H
#define MATERIAL_PAINTER_MODELRENDERER_H


#include "../../opengl/shader/Shader.h"
#include "../model/Model.h"

class ModelRenderer {
public:
    static void render(Model &model, Shader &shader);
private:
    static void render(Model::ModelNode &node, Shader &shader, glm::mat4 transform);
};


#endif //MATERIAL_PAINTER_MODELRENDERER_H
