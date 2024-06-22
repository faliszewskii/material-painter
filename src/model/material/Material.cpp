//
// Created by faliszewskii on 21.06.24.
//

#include "Material.h"

Material::Material(glm::vec4 albedo, std::optional<Texture> diffuseTexture, float shininess) :
albedo(albedo), diffuseTexture(diffuseTexture),shininess(shininess) {}

std::optional<Texture> Material::getDiffuseTexture() {
    return diffuseTexture;
}

glm::vec4 Material::getAlbedo() {
    return albedo;
}

glm::vec4& Material::getAlbedoRef() {
    return albedo;
}

float Material::getShininess() {
    return shininess;
}

float &Material::getShininessRef() {
    return shininess;
}