#include "bounds.h"

namespace rt3{


bool Bounds3f::box_intersect(const Ray &ray, pair<real_type, real_type> &hits) const{
  hits = {-INF, INF};

  for(int i = 0; i < 3; ++i){
    if(ray.d.at(i) == 0) continue;
    real_type invDir = 1.0 / ray.d.at(i);
    real_type minT = (minPoint.at(i) - ray.o.at(i)) * invDir;
    real_type maxT = (maxPoint.at(i) - ray.o.at(i)) * invDir;
    if(minT > maxT) swap(minT, maxT);

    hits.first = max(hits.first, minT);
    hits.second = min(hits.second, maxT);
  }
  
  return hits.first < hits.second;
}


bool Bounds3f::intersect_p(const Ray &ray, real_type maxT) const{
  pair<real_type, real_type> hits;
  bool intersects = box_intersect(ray, hits);
  if(!intersects) return false;
  else{
    if(hits.first > 0) return hits.first < maxT;
    else if(hits.second > 0) return hits.second < maxT;
    else return false;
  }
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
