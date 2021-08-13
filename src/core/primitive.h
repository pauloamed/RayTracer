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

class BoundedPrimitive : public Primitive{
protected:
	Bounds3f boundingBox;
public:
	BoundedPrimitive(Bounds3f bb):boundingBox(bb){}
	virtual ~BoundedPrimitive(){}
	Bounds3f getBoundingBox() { return boundingBox; }
};


class AggregatePrimitive : public BoundedPrimitive{
public:
	vector<shared_ptr<BoundedPrimitive>> primitives;

	AggregatePrimitive(vector<shared_ptr<BoundedPrimitive>> &&prim):
		BoundedPrimitive(prim[0]->getBoundingBox()), primitives(std::move(prim)){

		for(size_t i = 1; i < prim.size(); ++i){
			boundingBox = Bounds3f::unite(boundingBox, prim[i]->getBoundingBox());
		}
	}

	virtual ~AggregatePrimitive(){};
};


class PrimList : public AggregatePrimitive{
public:

	PrimList(vector<shared_ptr<BoundedPrimitive>> &&prim):AggregatePrimitive(std::move(prim)){}

	~PrimList(){};

	bool intersect_p( const Ray& r, real_type maxT ) const override;

	bool intersect( const Ray& r, shared_ptr<ObjSurfel> &isect ) const override;

};


class BVHAccel : public AggregatePrimitive{
public:
	BVHAccel(vector<shared_ptr<BoundedPrimitive>> &&prim):AggregatePrimitive(std::move(prim)){}

	~BVHAccel(){};

	bool intersect_p( const Ray& r, real_type maxT ) const override;

	bool intersect( const Ray& r, shared_ptr<ObjSurfel> &isect ) const override;

	static shared_ptr<BVHAccel> build(vector<shared_ptr<BoundedPrimitive>> &&prim, int leafSize){
		vector<shared_ptr<BoundedPrimitive>> startList = std::move(prim);

		vector<shared_ptr<BVHAccel>> leaves;
		for(int i = 0; i < startList.size(); i += leafSize){
			vector<shared_ptr<BoundedPrimitive>> currLeaf;
			for(int j = 0; j < leafSize; ++j){
				if(i + j >= startList.size()) break;
				currLeaf.push_back(startList[i + j]);
			}

			shared_ptr<BVHAccel> newNode{ new BVHAccel(std::move(currLeaf)) };
			leaves.push_back(newNode);
		}

		auto& currList = leaves;

		while(currList.size() != 1){
			vector<shared_ptr<BVHAccel>> nextList;
			for(int i = 0; i < currList.size(); i += 2){
				shared_ptr<BVHAccel> newNode{
					new BVHAccel({currList[i], currList[i + 1]})
				};
				nextList.push_back(newNode);
			}
			currList = std::move(nextList);
		}
		return currList[0];
	}

};


class GeometricPrimitive : public BoundedPrimitive{
public:
	shared_ptr<Material> material;
	unique_ptr<Shape> shape;

	GeometricPrimitive(shared_ptr<Material> mat, unique_ptr<Shape> &&s):
		BoundedPrimitive(s->computeBounds()), material(mat), shape(std::move(s)){}

	~GeometricPrimitive(){};

	bool intersect_p( const Ray& r, real_type maxT ) const override;

	bool intersect( const Ray& r, shared_ptr<ObjSurfel> &isect ) const override;

	shared_ptr<Material> get_material() const{  return material; }
};

} // namespace rt3



#endif