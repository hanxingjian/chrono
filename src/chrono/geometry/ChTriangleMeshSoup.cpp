// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Alessandro Tasora, Radu Serban
// =============================================================================

#include <cstdio>

#include "chrono/geometry/ChTriangleMeshSoup.h"
#include "chrono_thirdparty/tinyobjloader/tiny_obj_loader.h"

namespace chrono {
namespace geometry {

// Register into the object factory, to enable run-time dynamic creation and persistence
CH_FACTORY_REGISTER(ChTriangleMeshSoup)

ChTriangleMeshSoup::ChTriangleMeshSoup(const ChTriangleMeshSoup& source) {
    m_triangles = source.m_triangles;
}

void ChTriangleMeshSoup::LoadWavefrontMesh(std::string filename) {
    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, filename.c_str());

    for (size_t i = 0; i < shapes.size(); i++) {
        assert((shapes[i].mesh.indices.size() % 3) == 0);
        for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
            auto& indices = shapes[i].mesh.indices;
            auto& positions = shapes[i].mesh.positions;
            int i0 = indices[3 * f + 0];
            int i1 = indices[3 * f + 1];
            int i2 = indices[3 * f + 2];
            auto v0 = ChVector<>(positions[3 * i0 + 0], positions[3 * i0 + 1], positions[3 * i0 + 2]);
            auto v1 = ChVector<>(positions[3 * i1 + 0], positions[3 * i1 + 1], positions[3 * i1 + 2]);
            auto v2 = ChVector<>(positions[3 * i2 + 0], positions[3 * i2 + 1], positions[3 * i2 + 2]);
            addTriangle(v0, v1, v2);
        }
    }
}

void ChTriangleMeshSoup::addTriangle(const ChVector<>& vertex0,
                                   const ChVector<>& vertex1,
                                   const ChVector<>& vertex2) {
    ChTriangle tri(vertex0, vertex1, vertex2);
    m_triangles.push_back(tri);
}

void ChTriangleMeshSoup::Transform(const ChVector<> displ, const ChMatrix33<> rotscale) {
    for (int i = 0; i < this->m_triangles.size(); ++i) {
        m_triangles[i].p1 = rotscale * m_triangles[i].p1;
        m_triangles[i].p1 += displ;
        m_triangles[i].p2 = rotscale * m_triangles[i].p2;
        m_triangles[i].p2 += displ;
        m_triangles[i].p3 = rotscale * m_triangles[i].p3;
        m_triangles[i].p3 += displ;
    }
}

void ChTriangleMeshSoup::ArchiveOUT(ChArchiveOut& marchive) {
    // version number
    marchive.VersionWrite<ChTriangleMeshSoup>();
    // serialize parent class
    ChTriangleMesh::ArchiveOUT(marchive);
    // serialize all member data:
    marchive << CHNVP(m_triangles);
}

void ChTriangleMeshSoup::ArchiveIN(ChArchiveIn& marchive) {
    // version number
    int version = marchive.VersionRead<ChTriangleMeshSoup>();
    // deserialize parent class
    ChTriangleMesh::ArchiveIN(marchive);
    // stream in all member data:
    marchive >> CHNVP(m_triangles);
}

}  // end namespace geometry
}  // end namespace chrono
