#include "OpenMeshUtils.h"

#include <iostream>
#include <vector>

void laplacianSmoothing(MyMesh *mesh, float alpha){
    // this vector stores the computed centers of gravity
    std::vector<MyMesh::Point>  cogs;
    std::vector<MyMesh::Point>::iterator cog_it;
    cogs.reserve(mesh->n_vertices());
    // smoothing mesh argv[1] times
    MyMesh::VertexIter          v_it, v_end(mesh->vertices_end());
    MyMesh::VertexVertexIter    vv_it;
    MyMesh::Point               cog;
    MyMesh::Scalar              valence;

    cogs.clear();
    for (v_it=mesh->vertices_begin(); v_it!=v_end; ++v_it)
    {
        cog[0] = cog[1] = cog[2] = valence = 0.0;

        for (vv_it=mesh->vv_iter( *v_it ); vv_it.is_valid(); ++vv_it)
        {
            cog += mesh->point( *vv_it );
            ++valence;
        }
        cogs.push_back(cog / valence);
    }

    for (v_it=mesh->vertices_begin(), cog_it=cogs.begin();
         v_it!=v_end; ++v_it, ++cog_it)
        if ( !mesh->is_boundary( *v_it ) )
            mesh->set_point( *v_it, (1-alpha) * mesh->point(*v_it) + alpha * (*cog_it) );
}