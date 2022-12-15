#include "AxisMesh.h"
#include "../libs/glad/glad.h"
#include "Shader.h"

AxisMesh::AxisMesh() {
    m_vertices.emplace_back(VertexColor{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    m_vertices.emplace_back(VertexColor{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    m_vertices.emplace_back(VertexColor{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    m_vertices.emplace_back(VertexColor{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    m_vertices.emplace_back(VertexColor{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)});
    m_vertices.emplace_back(VertexColor{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)});

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(VertexColor), &m_vertices[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void *) offsetof(VertexColor, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

AxisMesh::~AxisMesh() {
    m_vertices.clear();
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void AxisMesh::Draw(Shader &shader, unsigned int primitive_type) {
    shader.useShaderProgram();
    shader.setMat4("model", getModel());

    glBindVertexArray(m_VAO);
    glDrawArrays(primitive_type, 0, 6);
}

