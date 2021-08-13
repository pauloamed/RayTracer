#ifndef BOUNDS_H
#define BOUNDS_H

#include "basic_types.h"
#include "rt3.h"

namespace rt3{


class Bounds3f{
private:
  // This is an axis-aligned 3d box. Therefore, for each axis, there are only two possible values.
  // (a box has 8 vertexes = 2^3 (for each of the 3 axis, there are 2 possible values))

  // minPoint will keep the minimum values for each axis
  // maxPoint will keep the maximum values for each axis
  // it is a non-degenerated box. Thus, values across the same axis are distincts
  Point3f minPoint, maxPoint; 
public:

  Bounds3f(){}

  bool intersect_p(const Ray &ray, pair<real_type, real_type> &hits) const;

  static Bounds3f unite(const Bounds3f &a, const Bounds3f &b);
};



}




#endif