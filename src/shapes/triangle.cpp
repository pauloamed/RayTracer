#include "triangle.h"


namespace rt3{


bool Triangle::intersect_p(const Ray &r, real_type maxT) const{
  return true;
}

bool Triangle::intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const{
  Vector3f edge[2] = {*v[1] - *v[0], *v[2] - *v[0]};

	Vector3f h = r.d.cross(edge[1]);
	
	auto a = edge[0] * h;
	if(abs(a) < EPS) return false; // This ray is parallel to this triangle.
	
	real_type f = 1 / a;
	Vector3f s = r.o - *v[0];

	real_type u = f * (s * h);
	if(u < 0.0 || u > 1.0) return false; // ??

	Vector3f q = s.cross(edge[0]);
	real_type v = f * (r.d * q);
	if (v < 0.0 || u + v > 1.0) return false;

	real_type t = f * (edge[1] * q);
	if (t > EPS){
		// normal tem que ser uma media das normais
		// 
		// isect = unique_ptr<ObjSurfel>(new ObjSurfel(contact, normal, r.d * -1, t[0]));	
		return true;
	}else return false;

}

vector<Shape *> create_triangle_list( shared_ptr<TriangleMesh> mesh){
	// List of shapes (triangles) we need to return to the client.
	vector<Shape *> tris;
	// Create the triangles, which are just references to the mesh database.
	for ( int i = 0 ; i < mesh->n_triangles ; ++i )
		tris.push_back(new Triangle( mesh, i ) );

	return tris;
}

}
