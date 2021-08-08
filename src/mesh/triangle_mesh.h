#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "../core/rt3-base.h"

namespace rt3{

/// This struct implements an indexd triangle mesh database.
struct TriangleMesh {
  int n_triangles; //!< # of triangles in the mesh.
  // The size of the three lists below should be 3 * nTriangles. Every 3 values we have a triangle.
  vector< int > vertex_indices;  //!< The list of indices to the vertex list, for each individual triangle.
  vector< int > normal_indices;  //!< The list of indices to the normals list, for each individual triangle.
  vector< int > uvcoord_indices; //!< The list of indices to the UV coord list, for each individual triangle.

  // Here comes the data associated with each vertex. WARNING: The length of the vectors below may differ.
  vector< shared_ptr<Point3f> > vertices;  //!< The 3D geometric coordinates
  vector< shared_ptr<Normal3f> > normals;  //!< The 3D normals.
  vector< shared_ptr<Point2f> > uvcoords; //!< The 2D texture coordinates.

  // Regular constructor (almost all data is imported via move semantics).
  TriangleMesh(){/*empty*/};
  /// Copy constructor disabled
  TriangleMesh( const TriangleMesh& ) = delete;
  /// Assign operator disabled.
  TriangleMesh& operator=( const TriangleMesh & ) = delete;
  /// Move constructor.
  TriangleMesh( TriangleMesh && other ) = delete;
};

}



#endif