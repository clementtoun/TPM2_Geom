#include "TriMesh.h"

#include "../libs/glad/glad.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "Shader.h"
#include <algorithm>

void TriMesh::Draw(Shader &shader, unsigned int primitive_type) {
    shader.useShaderProgram();
    shader.setMat4("model", getModel());

    glBindVertexArray(m_VAO);
    glDrawElements(primitive_type, static_cast<int>(m_n_indices), GL_UNSIGNED_INT, nullptr);
}

bool TriMesh::readMeshFromFile(const char *filename) {
    if (!OpenMesh::IO::read_mesh(m_mesh, filename))
    {
        std::cerr << "read mesh error\n";
        return false;
    }

    m_mesh.request_vertex_normals();
    m_mesh.request_face_normals();
    m_mesh.request_vertex_status();

    OpenMesh::VPropHandleT<float> f_prop;
    m_mesh.add_property(f_prop, "f");
    for(auto vertex : m_mesh.vertices()){
        m_mesh.property(f_prop, vertex) = 0.0;
        m_mesh.status(vertex).set_selected(0);
    }

    setupMesh();
    commitChange();

    return true;
}

void TriMesh::normalize_center() {
    auto size = m_aabb.max - m_aabb.min;
    auto center = (m_aabb.max + m_aabb.min) / 2.f;
    scale(glm::vec3(1.f / std::max(std::max(size.x, size.y), size.z)));
    translate(-center);
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

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, is_selected));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, f));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

}

bool TriMesh::commitChange() {
    m_vertices = std::vector<Vertex>(m_mesh.n_vertices());
    m_indices = std::vector<unsigned int>(0);

    m_mesh.update_normals();

    auto first_vertex = m_mesh.points()[0];
    m_aabb.min = glm::vec3(first_vertex[0], first_vertex[1], first_vertex[2]);
    m_aabb.max = glm::vec3(first_vertex[0], first_vertex[1], first_vertex[2]);

    OpenMesh::VPropHandleT<float> f_prop;
    m_mesh.get_property_handle(f_prop, "f");

    for (auto vertex : m_mesh.vertices()) {
        auto point = m_mesh.point(vertex);
        m_aabb.min.x = m_aabb.min.x > point[0] ? point[0] : m_aabb.min.x;
        m_aabb.min.y = m_aabb.min.y > point[1] ? point[1] : m_aabb.min.y;
        m_aabb.min.z = m_aabb.min.z > point[2] ? point[2] : m_aabb.min.z;
        m_aabb.max.x = m_aabb.max.x < point[0] ? point[0] : m_aabb.max.x;
        m_aabb.max.y = m_aabb.max.y < point[1] ? point[1] : m_aabb.max.y;
        m_aabb.max.z = m_aabb.max.z < point[2] ? point[2] : m_aabb.max.z;

        m_vertices[vertex.idx()] = Vertex{point, m_mesh.normal(vertex), m_mesh.status(vertex).selected(), m_mesh.property(f_prop, vertex)};
    }

    for (auto face : m_mesh.faces()) {
        for (auto vertex : m_mesh.fv_cw_range(face)) {
            m_indices.emplace_back((unsigned int) vertex.idx());
        }
    }

    m_n_indices = m_indices.size();

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_DYNAMIC_DRAW);

    return true;
}

MyMesh* TriMesh::getMesh() {
    return &m_mesh;
}

unsigned int TriMesh::getVao() const {
    return m_VAO;
}

unsigned int TriMesh::getNIndices() const {
    return m_n_indices;
}

const AABB &TriMesh::getAABB() const {
    return m_aabb;
}

int TriMesh::intersectPoint(Ray &r) {
    float tmin = 100000.0f;
    OpenMesh::VertexHandle vhmin;
    float boxwidth = 0.005f;
    bool ok = false;
    for (auto vertex : m_mesh.vertices()) {
        auto vp = m_mesh.point(vertex);
        auto p = getModel() * glm::vec4(vp[0], vp[1], vp[2], 1.0f);
        AABB Paabb = {glm::vec3(p.x, p.y, p.z) - boxwidth / 2.0f, glm::vec3(p.x, p.y, p.z) + boxwidth / 2.0f};
        bool intersect = intersectBox(Paabb, r);
        if (intersect && r.t < tmin) {
            vhmin = vertex;
            tmin = r.t;
            ok = true;
        }
    }
    if(ok){
        m_mesh.status(vhmin).set_selected(1 - m_mesh.status(vhmin).selected());
        commitChange();
        return vhmin.idx();
    }
    return -1;
}

void TriMesh::deform(float factor) {
    OpenMesh::VPropHandleT<float> f_prop;
    m_mesh.get_property_handle(f_prop, "f");

    for(auto vertex : m_mesh.vertices()){
        m_mesh.set_point(vertex, m_mesh.point(vertex) + m_mesh.normal(vertex) * m_mesh.property(f_prop, vertex) * factor);
    }

    commitChange();
}
