#ifndef TPM2_GEOM_TRIMESH_H
#define TPM2_GEOM_TRIMESH_H

#include "Drawable.h"
#include <vector>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;

struct Vertex {
    OpenMesh::Vec3f vertices;
    OpenMesh::Vec3f Normal;
};

class TriMesh : public Drawable {

public:

    ~TriMesh();

    void Draw(Shader &shader) override;

    bool readMeshFromFile(const char* filename);

    bool commitChange();

    MyMesh* getMMesh();

private:
    void setupMesh();
    void clear();

    unsigned int m_VAO, m_VBO, m_EBO;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    unsigned int m_n_indices;
    MyMesh m_mesh;
};


#endif //TPM2_GEOM_TRIMESH_H
