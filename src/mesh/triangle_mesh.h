#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "../core/rt3-base.h"

namespace rt3{

/// This struct implements an indexd triangle mesh database.
struct TriangleMesh {
  int n_triangles; //!< # of triangles in the mesh.
  // The size of the three lists below should be 3 * nTriangles. Every 3 values we have a triangle.
  shared_ptr<vector<int>> vertex_indices;  //!< The list of indices to the vertex list, for each individual triangle.
  shared_ptr<vector<int>> normal_indices;  //!< The list of indices to the normals list, for each individual triangle.
  shared_ptr<vector<int>> uvcoord_indices; //!< The list of indices to the UV coord list, for each individual triangle.

  // Here comes the data associated with each vertex. WARNING: The length of the vectors below may differ.
  shared_ptr<vector<shared_ptr<Point3f>>> vertices;  //!< The 3D geometric coordinates
  shared_ptr<vector<shared_ptr<Normal3f>>> normals;  //!< The 3D normals.
  shared_ptr<vector<shared_ptr<Point2f>>> uvcoords; //!< The 2D texture coordinates.

  // Regular constructor
  TriangleMesh()
  :vertex_indices(new vector<int>), normal_indices(new vector<int>), uvcoord_indices(new vector<int>),
  vertices(new vector<shared_ptr<Point3f>>()), normals(new vector<shared_ptr<Normal3f>>()), uvcoords(new vector<shared_ptr<Point2f>>())
  {/*empty*/};

  TriangleMesh(
    int n,
    shared_ptr<vector<int>> indexes,
    shared_ptr<vector<int>> uv_indexes,
    shared_ptr<vector<shared_ptr<Point3f>>> vertexes,
    shared_ptr<vector<shared_ptr<Normal3f>>> normal,
    shared_ptr<vector<shared_ptr<Point2f>>> uvcrds
  ):n_triangles(n), vertex_indices(indexes), normal_indices(indexes), uvcoord_indices(uv_indexes), 
  vertices(vertexes), normals(normal), uvcoords(uvcrds){}


  /// Copy constructor disabled
  TriangleMesh( const TriangleMesh& ) = delete;
  /// Assign operator disabled.
  TriangleMesh& operator=( const TriangleMesh & ) = delete;
  /// Move constructor.
  TriangleMesh( TriangleMesh && other ) = delete;
};


TriangleMesh *create_triangle_mesh(const ParamSet &ps);

Normal3f compute_normals();


}



#endif