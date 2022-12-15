#ifndef TPM2_GEOM_MESH_FONCTION_PROPAGATOR_H
#define TPM2_GEOM_MESH_FONCTION_PROPAGATOR_H

#include "TriMesh.h"

class mesh_fonction_propagator {

public:
    mesh_fonction_propagator() = default;

    void set_zero_index();
    void set_one_index();

    void setMesh(MyMesh* mMesh);

    void propagateMatrix();
    void propagate();

private:
    MyMesh* m_mesh;
    std::vector<OpenMesh::Vec3f> m_points;
    std::vector<OpenMesh::Vec3f> m_normals;
    std::vector<unsigned int> m_zero_index;
    std::vector<unsigned int> m_one_index;
};


#endif //TPM2_GEOM_MESH_FONCTION_PROPAGATOR_H
