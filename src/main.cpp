#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <Eigen/Dense>
#include <iostream>

#include "myApp.h"

typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;
using Eigen::MatrixXd;

int main(int argc, char **argv){

    Application* app = new myApp("TPM2_Geom", 1080, 720);
    app->Run();
    delete app;

    return 0;
}

