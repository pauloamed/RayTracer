#include "triangle_mesh.h"

namespace rt3{

TriangleMesh *create_triangle_mesh(const ParamSet &ps){

  auto n = retrieve(ps, "ntriangles", 1);
  auto indices = retrieve(ps, "indices", shared_ptr<vector<int>>());

  auto vertices = retrieve(ps, "vertices", shared_ptr<vector<shared_ptr<Point3f>>>());
  auto normals = retrieve(ps, "normals", shared_ptr<vector<shared_ptr<Normal3f>>>());
  auto uv = retrieve(ps, "uv", shared_ptr<vector<shared_ptr<Point2f>>>());

  return new TriangleMesh(n, indices, vertices, normals, uv);
}

Normal3f compute_normals(){
  return Normal3f();
}

}