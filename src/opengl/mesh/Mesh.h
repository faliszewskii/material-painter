//
// Created by faliszewskii on 18.04.24.
//

#ifndef PUMAROBOT_MESH_H
#define PUMAROBOT_MESH_H

#include "../../glew_glfw.h"
#include <optional>
#include "is_vertex.h"

template<typename TVertex> requires is_vertex<TVertex>
class Mesh {
    int drawingMode;
    int indicesCount;
    int verticesCount;
public:

    explicit Mesh(std::vector<TVertex> vertices = std::vector<TVertex>(), std::optional<std::vector<unsigned int>> indices = {},
                  int drawingMode = GL_TRIANGLES) : verticesCount(vertices.size()), indicesCount(indices.has_value() ? indices->size() : 0), drawingMode(drawingMode) {
        setupMesh(std::move(vertices), std::move(indices));
    }

    void render(int instanceCount = 1) const {
        // draw mesh
        glBindVertexArray(VAO);
        if (indicesCount != 0)
            glDrawElementsInstanced(drawingMode, indicesCount, GL_UNSIGNED_INT, 0, instanceCount);
        else
            glDrawArraysInstanced(drawingMode, 0, verticesCount,instanceCount);
        glBindVertexArray(0);
    }

    void update(std::vector<TVertex> &&newVertices, std::optional<std::vector<unsigned int>> &&newIndices = {}) {

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, newVertices.size() * sizeof(TVertex), &newVertices[0], GL_STATIC_DRAW);

        if (newIndices) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, newIndices->size() * sizeof(unsigned int), &newIndices.value()[0],
                         GL_STATIC_DRAW);
        }

        glBindVertexArray(0);
    }

    void update(TVertex vertex, int i) {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(TVertex), sizeof(TVertex), &vertex);

        glBindVertexArray(0);
    }

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh(std::vector<TVertex> vertices, std::optional<std::vector<unsigned int>> indices) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TVertex), &vertices[0], GL_STATIC_DRAW);

        if (indices) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.value().size() * sizeof(unsigned int), &indices.value()[0],
                         GL_STATIC_DRAW);
        }

        for(int i = 0; i < TVertex::getSizes().size(); i++) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, TVertex::getSizes()[i], TVertex::getTypes()[i], GL_FALSE,
                                  sizeof(TVertex), reinterpret_cast<void*>(TVertex::getOffsets()[i]));
        }
        glBindVertexArray(0);
    }
};


#endif //PUMAROBOT_MESH_H
