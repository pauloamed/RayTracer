#include "triangle_mesh.h"

namespace rt3{

TriangleMesh *create_triangle_mesh(const ParamSet &ps){

  auto n = retrieve(ps, "ntriangles", 1);
  auto indices = retrieve(ps, "indices", shared_ptr<vector<int>>(new vector<int>));

  auto vertices = retrieve(ps, "vertices", shared_ptr<vector<shared_ptr<Point3f>>>(new vector<shared_ptr<Point3f>>)); 
  auto normals = retrieve(ps, "normals", shared_ptr<vector<shared_ptr<Normal3f>>>(new vector<shared_ptr<Normal3f>>));
  auto uv = retrieve(ps, "uv", shared_ptr<vector<shared_ptr<Point2f>>>(new vector<shared_ptr<Point2f>>));

  if(indices->size() % 3 != 0 || indices->size() != n * 3){
    RT3_ERROR("Indices size doesnt match num. of triangles!");
  }
 
  if(retrieve(ps, "reverse_vertex_order", false)){
    auto it = indices->begin();
    while(it != indices->end()){
      reverse(it, it + 3);
      it += 3;
    }
  }

  if(retrieve(ps, "compute_normals", true)){
    RT3_ERROR("Not implemented.");
  }

  return new TriangleMesh(n, indices, vertices, normals, uv);
}

Normal3f compute_normals(shared_ptr<Point3f> a, shared_ptr<Point3f> b, shared_ptr<Point3f> c){
  Vector3f edges[2] = {*a - *b, *c - *a};
  return edges[0].cross(edges[1]);
}

}