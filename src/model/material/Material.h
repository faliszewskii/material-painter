//
// Created by faliszewskii on 21.06.24.
//

#ifndef MATERIAL_PAINTER_MATERIAL_H
#define MATERIAL_PAINTER_MATERIAL_H


#include <glm/vec4.hpp>
#include <optional>
#include "../../opengl/texture/Texture.h"

class Material {
    glm::vec4 albedo;
    std::optional<Texture> diffuseTexture;
    float shininess;
public:
    explicit Material(glm::vec4 albedo = glm::vec4(1.0f),
                      std::optional<Texture> diffuseTexture = {},
                      float shininess = 0);

    std::optional<Texture> getDiffuseTexture();

    glm::vec4 getAlbedo();
    glm::vec4& getAlbedoRef();

    float getShininess();
    float &getShininessRef();
};


#endif //MATERIAL_PAINTER_MATERIAL_H
