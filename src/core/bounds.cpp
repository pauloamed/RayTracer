#include "bounds.h"

namespace rt3{


bool Bounds3f::intersect_p(const Ray &ray, pair<real_type, real_type> &hits) const{
  hits = {-INF, INF};

  for(int i = 0; i < 3; ++i){
    if(ray.d.at(i) == 0) continue;
    real_type invDir = ray.d.at(i);
    real_type minT = (minPoint.at(i) - ray.o.at(i)) * invDir;
    real_type maxT = (maxPoint.at(i) - ray.o.at(i)) * invDir;
    if(minT > maxT) swap(minT, maxT);

    hits.first = max(hits.first, minT);
    hits.second = min(hits.second, maxT);
  }

  return hits.first < hits.second;
}

Bounds3f Bounds3f::unite(const Bounds3f &a, const Bounds3f &b){
  Bounds3f x(a);
  for(int i = 0; i < 3; ++i){
    x.minPoint[i] = min(a.minPoint.at(i), b.minPoint.at(i));
    x.maxPoint[i] = max(a.maxPoint.at(i), b.maxPoint.at(i));
  }
  return x;
}



}
