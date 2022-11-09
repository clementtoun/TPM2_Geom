#include "TriMesh.h"

#include "../libs/glad/glad.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "Shader.h"

void TriMesh::Draw(Shader &shader) {
    shader.useShaderProgram();
    for(auto & i : m_model) {
        shader.setMat4("model", i);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, static_cast<int>(m_n_indices), GL_UNSIGNED_INT, nullptr);
    }
}

bool TriMesh::readMeshFromFile(const char *filename) {
    if (!OpenMesh::IO::read_mesh(m_mesh, filename))
    {
        std::cerr << "read mesh error\n";
        return false;
    }

    commitChange();

    return true;
}

TriMesh::~TriMesh() {
    m_vertices.clear();
    m_indices.clear();
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void TriMesh::setupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

bool TriMesh::commitChange() {
    m_vertices = std::vector<Vertex>(0);
    m_indices = std::vector<unsigned int>(0);

    m_mesh.request_face_normals();
    m_mesh.request_vertex_normals();
    m_mesh.update_normals();
    for (auto vertex : m_mesh.vertices()) {
        m_vertices.emplace_back(Vertex{m_mesh.point(vertex),m_mesh.normal(vertex)});
    }

    for (auto face : m_mesh.faces()) {
        for (auto vertex : m_mesh.fv_cw_range(face)) {
            m_indices.emplace_back((unsigned int) vertex.idx());
        }
    }

    m_n_indices = m_indices.size();

    setupMesh();

    m_vertices.clear();
    m_indices.clear();

    return true;
}

MyMesh* TriMesh::getMMesh() {
    return &m_mesh;
}
