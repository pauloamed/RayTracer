#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "rt3.h"
#include "basic_types.h"
#include "shape.h"


namespace rt3{

class Primitive {
public:
	
	virtual ~Primitive(){};
	virtual bool intersect( const Ray& r, Surfel *sf ) const = 0;
	
	virtual bool intersect_p( const Ray& r ) const = 0;
	virtual shared_ptr<Material> get_material() const = 0;
protected:
	std::shared_ptr<Material> material;
};


class GeometricPrimitive : public Primitive{
public:
	shared_ptr<Material> material;
	unique_ptr<Shape> shape;

	GeometricPrimitive(shared_ptr<Material> mat, unique_ptr<Shape> &&s):
		material(mat), shape(std::move(s)){}

	~GeometricPrimitive(){};

	bool intersect_p( const Ray& r ) const override;

	bool intersect( const Ray& r, Surfel *sf ) const override;

	shared_ptr<Material> get_material() const override{ 
		return material;
	}
};


class AggregatePrimitive : public Primitive{
public:
	shared_ptr<Material> get_material(void) const override{ 
		RT3_ERROR("Can't retrieve material from aggregate!");
		return nullptr;
	}
	virtual ~AggregatePrimitive(){};
};


class PrimList : public AggregatePrimitive{
public:
	vector<unique_ptr<Primitive>> primitives;

	PrimList(vector<unique_ptr<Primitive>> &&prim):
		primitives(std::move(prim)){}

	~PrimList(){};

	bool intersect_p( const Ray& r ) const override;

	bool intersect( const Ray& r, Surfel *sf ) const override;

	using AggregatePrimitive::get_material;

};

} // namespace rt3



#endif