#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../core/shape.h"
#include "../mesh/triangle_mesh.h"

namespace rt3{

/// Represents a single triangle.
class Triangle : public Shape {
private:
  shared_ptr<const Point3f> v[3];
  shared_ptr<const Normal3f> n[3];
  shared_ptr<const Point2f> uv[3];

  shared_ptr<TriangleMesh> mesh; //!< This is the **indexed triangle mesh database** this triangle is linked to.
  bool backface_cull;            //!< Set it `true` to activate backface culling.



public:
  // The single constructor, that receives the mesh, this triangle id, and an indication for backface culling.
  Triangle( shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc=true )
    : Shape(), mesh{mesh}, backface_cull{bfc}
  {
    // This is just a shortcut to access this triangle's data stored in the mesh database.
    for(int i = 0; i < 3; ++i){
      v[i] = mesh->vertices[mesh->vertex_indices[ 3 * tri_id + i]];
      n[i] = mesh->normals[mesh->normal_indices[ 3 * tri_id + i]];
      uv[i] = mesh->uvcoords[mesh->uvcoord_indices[ 3 * tri_id + i]];
    }
  }


  /// Return the triangle's bounding box.
  // Bounds3f object_bound() const;
  /// The regular intersection methods, as defined in the Shape parent class.
  bool intersect_p(const Ray &r, real_type maxT) const override;
  bool intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const override;

  /// This friend function helps us debug the triangles, if we want to.
  friend std::ostream& operator<<( std::ostream& os, const Triangle & t );
};


/// This function creates the internal data structure, required by the RT3.
vector<shared_ptr<Shape>> create_triangle_list( shared_ptr<TriangleMesh> mesh, bool backface_cull );

}

#endif