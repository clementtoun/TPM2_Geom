#ifndef TPM2_GEOM_OPENMESHUTILS_H
#define TPM2_GEOM_OPENMESHUTILS_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

void laplacianSmoothing(MyMesh *mesh, float alpha);



#endif //TPM2_GEOM_OPENMESHUTILS_H
