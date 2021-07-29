#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "rt3.h"
#include "basic_types.h"
#include "shape.h"


namespace rt3{

class Primitive {
public:
	virtual ~Primitive(){};
	virtual bool intersect( const Ray& r, shared_ptr<ObjSurfel> &isect ) const = 0;
	virtual bool intersect_p( const Ray& r, real_type maxT ) const = 0;
};


class GeometricPrimitive : public Primitive{
public:
	shared_ptr<Material> material;
	unique_ptr<Shape> shape;

	GeometricPrimitive(shared_ptr<Material> mat, unique_ptr<Shape> &&s):
		material(mat), shape(std::move(s)){}

	~GeometricPrimitive(){};

	bool intersect_p( const Ray& r, real_type maxT ) const override;

	bool intersect( const Ray& r, shared_ptr<ObjSurfel> &isect ) const override;

	shared_ptr<Material> get_material() const{ 
		return material;
	}
};


class AggregatePrimitive : public Primitive{
public:
	virtual ~AggregatePrimitive(){};
};


class PrimList : public AggregatePrimitive{
public:
	vector<shared_ptr<Primitive>> primitives;

	PrimList(vector<shared_ptr<Primitive>> &&prim):
		primitives(std::move(prim)){}

	~PrimList(){};

	bool intersect_p( const Ray& r, real_type maxT ) const override;

	bool intersect( const Ray& r, shared_ptr<ObjSurfel> &isect ) const override;

};

} // namespace rt3



#endif