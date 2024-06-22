//
// Created by faliszewskii on 22.06.24.
//

#ifndef MATERIAL_PAINTER_TRANSFORMATION_H
#define MATERIAL_PAINTER_TRANSFORMATION_H


#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <variant>

class Transformation {
    struct TripleTransform {
        glm::vec3 translation = glm::vec3(0.0f);
        glm::quat rotation = glm::identity<glm::quat>();
        glm::vec3 scale = glm::vec3(1.0f);
    };
//    std::variant<TripleTransform, glm::mat4> transform;
    glm::mat4 transform;
public:
    Transformation() : transform(glm::mat4(1.0f)) {}
    explicit Transformation(glm::mat4 transform) : transform(transform) {}
//    Transformation(glm::vec3 translation, glm::quat rotation, glm::vec3 scale) : transform(TripleTransform{translation, rotation, scale}) {}

    glm::mat4 getTransform() {
        return transform;
    }
};


#endif //MATERIAL_PAINTER_TRANSFORMATION_H
