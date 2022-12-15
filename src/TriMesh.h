#ifndef TPM2_GEOM_TRIMESH_H
#define TPM2_GEOM_TRIMESH_H

#include "Drawable.h"
#include <vector>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <Eigen/Dense>
#include "RayUtils.h"

typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;

struct Vertex {
    OpenMesh::Vec3f vertices;
    OpenMesh::Vec3f Normal;
    unsigned int is_selected;
    float f;
};

class TriMesh : public Drawable {

public:

    ~TriMesh() override;

    void Draw(Shader &shader, unsigned int primitive_type) override;

    bool readMeshFromFile(const char* filename);

    bool commitChange();

    MyMesh* getMesh();

    unsigned int getNIndices() const;

    const AABB &getAABB() const;

    unsigned int getVao() const;

    void normalize_center();

    int intersectPoint(Ray &r);

    void deform(float factor);

private:
    void setupMesh();

    unsigned int m_VAO, m_VBO, m_EBO;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    unsigned int m_n_indices;
    MyMesh m_mesh;
    AABB m_aabb;
};


#endif //TPM2_GEOM_TRIMESH_H
