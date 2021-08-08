#include "triangle.h"


namespace rt3{

bool Triangle::intersect_p(const Ray &r, real_type maxT) const{
  return true;
}

bool Triangle::intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const{
  return true;
}

vector<shared_ptr<Shape>> create_triangle_list( shared_ptr<TriangleMesh> mesh, bool backface_cull ){
	// List of shapes (triangles) we need to return to the client.
	vector<shared_ptr<Shape>> tris;
	// Create the triangles, which are just references to the mesh database.
	for ( int i = 0 ; i < mesh->n_triangles ; ++i )
		tris.push_back(std::make_shared<Triangle>( mesh, i, backface_cull ) );

	return tris;
}

}
