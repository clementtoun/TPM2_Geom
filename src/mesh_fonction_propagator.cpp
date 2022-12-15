#include "mesh_fonction_propagator.h"
#include <algorithm>
#include <Eigen/Dense>

void mesh_fonction_propagator::set_zero_index() {
    OpenMesh::VPropHandleT<float> f_prop;
    m_mesh->get_property_handle(f_prop, "f");

    m_zero_index = std::vector<unsigned int>(0);
    for(auto vertex : m_mesh->vertices()){
        m_mesh->property(f_prop, vertex) = 0.;
        if(m_mesh->status(vertex).selected()) {
            m_zero_index.emplace_back(vertex.idx());
            m_mesh->status(vertex).set_selected(0);
        }
    }
}

void mesh_fonction_propagator::set_one_index() {
    m_one_index = std::vector<unsigned int>(0);
    m_points = std::vector<OpenMesh::Vec3f>(m_mesh->n_vertices());
    m_normals = std::vector<OpenMesh::Vec3f>(m_mesh->n_vertices());
    for(auto vertex : m_mesh->vertices()){
        if(m_mesh->status(vertex).selected()) {
            m_one_index.emplace_back(vertex.idx());
            m_mesh->status(vertex).set_selected(0);
        }
        m_points[vertex.idx()] = m_mesh->point(vertex);
        m_normals[vertex.idx()] = m_mesh->normal(vertex);
    }
}

void mesh_fonction_propagator::setMesh(MyMesh* mMesh) {
    m_mesh = mMesh;
}

using Eigen::MatrixXf;
using Eigen::VectorXf;

void mesh_fonction_propagator::propagateMatrix() {

    MatrixXf M(m_mesh->n_vertices(), m_mesh->n_vertices());
    M.setZero();
    VectorXf B(m_mesh->n_vertices());
    B.setZero();

    MyMesh::VertexVertexIter vv_it;

    for(auto vertice : m_mesh->vertices()){
        int index = vertice.idx();
        if(std::find(m_zero_index.begin(), m_zero_index.end(), index) != m_zero_index.end()){
            M(index, index) = 1.f;
            B(index) = 0.f;
        }
        else if(std::find(m_one_index.begin(), m_one_index.end(), index) != m_one_index.end()){
            M(index, index) = 1.f;
            B(index) = 1.f;
        }
        else{
            M(index, index) = -1.f;
            B(index) = 0.f;
            uint valance = vertice.valence();
            for (vv_it=m_mesh->vv_iter( vertice ); vv_it.is_valid(); ++vv_it) {
                int index_fa = vv_it->idx();
                M(index, index_fa) = 1.f/float(valance);
            }
        }
    }

    Eigen::ColPivHouseholderQR<MatrixXf> dec(M);
    VectorXf X = dec.solve(B);

    OpenMesh::VPropHandleT<float> f_prop;
    m_mesh->get_property_handle(f_prop, "f");

    std::cout << X << std::endl;

    for(auto vertex : m_mesh->vertices()){
        m_mesh->property(f_prop, vertex) += X(vertex.idx());
    }
}

void mesh_fonction_propagator::propagate() {

    OpenMesh::VPropHandleT<float> f_prop;
    m_mesh->get_property_handle(f_prop, "f");

    std::vector<float>  cogs;
    std::vector<float>::iterator cog_it;
    cogs.reserve(m_mesh->n_vertices());
    MyMesh::VertexIter          v_it, v_end(m_mesh->vertices_end());
    MyMesh::VertexVertexIter    vv_it;
    float cog, valence;

    for(auto vertice : m_mesh->vertices()){
        if(std::find(m_one_index.begin(), m_one_index.end(), vertice.idx()) != m_one_index.end()){
            m_mesh->property(f_prop, vertice) = 1.f;
        }
    }

    for(v_it=m_mesh->vertices_begin(); v_it!=v_end; ++v_it){
        cog = valence = 0.f;
        for (vv_it=m_mesh->vv_iter( *v_it ); vv_it.is_valid(); ++vv_it)
        {
            cog += m_mesh->property(f_prop, *vv_it);
            ++valence;
        }
        cogs.push_back(cog / valence);
    }

    for (v_it=m_mesh->vertices_begin(), cog_it=cogs.begin();
         v_it!=v_end; ++v_it, ++cog_it) {
        m_mesh->property(f_prop, *v_it) = (*cog_it);
    }
}
