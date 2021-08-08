#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../core/shape.h"
#include "../mesh/triangle_mesh.h"

namespace rt3{

/// Represents a single triangle.
class Triangle : public Shape {
private:
  int *v; //!< Pointer to the vertex index array stored in the triangle mesh database. 
  // After the proper triangle initialization, we may access vertices with v[0], v[1], and v[2]
  int *n; //!< Pointer to the normal index array stored in the triangle mesh database.
  // After the proper triangle initialization, we may access normals with n[0], n[1], and n[2]
  int *uv;//!< Pointer to the UV coord index array stored in the triangle mesh database.
  // After the proper triangle initialization, we may access UV coords with uv[0], uv[1], and uv[2]

  shared_ptr<TriangleMesh> mesh; //!< This is the **indexed triangle mesh database** this triangle is linked to.
  bool backface_cull;            //!< Set it `true` to activate backface culling.

public:
  // The single constructor, that receives the mesh, this triangle id, and an indication for backface culling.
  Triangle( shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc=true )
    : Shape(), mesh{mesh}, backface_cull{bfc}
  {
    // This is just a shortcut to access this triangle's data stored in the mesh database.
    v  = &mesh->vertex_indices[ 3 * tri_id ];
    n  = &mesh->normal_indices[ 3 * tri_id ];
    uv = &mesh->uvcoord_indices[ 3 * tri_id ];
  }
  /// Return the triangle's bounding box.
  // Bounds3f object_bound() const;
  /// The regular intersection methods, as defined in the Shape parent class.
  bool intersect_p(const Ray &r, real_type maxT) const override;
  bool intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const override;

  /// This friend function helps us debug the triangles, if we want to.
  friend std::ostream& operator<<( std::ostream& os, const Triangle & t );
};

}

#endif