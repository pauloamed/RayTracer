#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include "error.h"
#include <sstream>

using std::array;
using std::string;
using std::vector;
using std::copy_n;

namespace rt3{

//=== aliases
typedef float real_type;
typedef size_t size_type;
typedef std::tuple<bool, std::string> result_type;


// wrapper class for structured point values
template <typename T, int size> class StructuredValues {
protected:
  array<T, size> values;

public:
  StructuredValues() = default;
  StructuredValues(const StructuredValues &clone) = default;

  StructuredValues(const vector<T> &_values){
    assert(values.size() == size);
    copy_n(_values.begin(), _values.size(), values.begin());
  }  

  T& operator[]( const int i ){
    assert(i < size);
    return values[i];
  }

  T at(int i) const{
    assert(i < size);
    return values[i];
  }

  inline T x() const{ return values[0]; }
  inline T y() const{ return values[1]; }
  inline T z() const{ 
    assert(size == 3);
    return values[2]; 
  }

  string toString() const{
    std::stringstream ss; ss << "( ";
    for(int i = 0; i < size; ++i){
      ss << values[i] << " ";
    }
    ss << ")";
    return ss.str();
  }

};

template<typename T, int size> struct Vector : 
    StructuredValues<T, size>{
  private:
    using StructuredValues<T, size>::values;
  public:
  using StructuredValues<T, size>::StructuredValues;
  
  Vector():StructuredValues<T,size>(){}
  Vector(const Vector &clone):StructuredValues<T,size>(clone){}
  Vector(const vector<T> &_values):StructuredValues<T,size>(_values){}
    
    Vector operator*(real_type t) const{
        Vector v(*this);
        for(auto &x : v.values) x *= t;
        return v;
    }

    real_type operator*(const Vector &u) const{
      real_type prod = 0;
      for(int i = 0; i < size; ++i){
        prod += this->at(i) * u.at(i);
      }
      return prod;
    }

    Vector normalize() const{
      Vector v(*this);
      real_type norm = v.getNorm();
      return v * (1.0 / norm);
    }

    Vector cross(const Vector &x) const{
      Vector v(*this);
      v[0] = this->at(1) * x.at(2) - this->at(2) * x.at(1);
      v[1] = this->at(0) * x.at(2) - this->at(2) * x.at(0);
      v[2] = this->at(0) * x.at(1) - this->at(1) * x.at(0);
      return v;
    }

    real_type getNorm() const{
      real_type norm = 0;
      for(auto x : values){
        norm += x * x;
      }
      return sqrt(norm);
    }
};


template<typename T, int size> struct Point : 
    StructuredValues<T, size>{

  using StructuredValues<T, size>::StructuredValues;
  Point():StructuredValues<T,size>(){}
  Point(const Point &clone):StructuredValues<T,size>(clone){}
  Point(const vector<T> &_values):StructuredValues<T,size>(_values){}
};


template<typename T, int size>
Vector<T,size> operator-(const Point<T,size>& x, const Point<T,size>& y){
    Vector<T,size> v;
    for(int i = 0; i < size; ++i){
      v[i] = x.at(i) - y.at(i);
    }
    // std::cout << "- " << x.toString() << " " << y.toString() << " " << v.toString() << std::endl;
    return v;
}


template<typename T, int size>
Point<T, size> operator+(const Point<T, size> x, const Vector<T,size> y){
    Point<T, size> v(x);
    for(int i = 0; i < size; ++i){
        v[i] += y.at(i);
    }
    // std::cout << "+ " << x.toString() << " " << y.toString() << " " << v.toString() << std::endl;
    return v;
}


template<typename T, int size>
Vector<T, size> operator+(const Vector<T, size> x, const Vector<T,size> y){
    Vector<T, size> v;
    for(int i = 0; i < size; ++i){
        v[i] = x.at(i) + y.at(i);
    }
    // std::cout << "+ " << x.toString() << " " << y.toString() << " " << v.toString() << std::endl;
    return v;
}

// Point aliases
using Point3i = Point<int, 3>;
using Point2i = Point<int, 2>;
using Point2f = Point<float, 2>;
using Point3f = Point<float, 3>;

// Vector aliases
using Vector3f = Vector<float, 3>;
using Vector3i = Vector<int, 3>;
using Normal3f = Vector<float, 3>;

// Other types
using Color = StructuredValues<float, 3>;
using ColorXYZ = Color;
using Spectrum = StructuredValues<float, 3>;


// List of points
using ListPoint3f = std::vector<Point3f>;


class Ray {
    public:
        Ray (const Point3f& _o, const Vector3f& _d ) : o{_o}, d{_d} {
          assert(d.at(2) == _d.at(2));
          assert(d.at(1) == _d.at(1));
          assert(d.at(0) == _d.at(0));
        }
        Point3f o; //!< origin
        Vector3f d; //!< direction
        Point3f operator()(real_type t) const { return o + d; }
};


struct ScreenWindow{
    real_type left, right;
    real_type top, bottom;

    ScreenWindow() = default;

    ScreenWindow(real_type l, real_type r, real_type t, real_type b):
    left(l), right(r), top(t), bottom(b){}

    ScreenWindow(const vector<real_type> &vals):
      ScreenWindow(vals[0], vals[1], vals[2], vals[3]){}

    real_type width() const { return right - left; }
    real_type height() const { return top - bottom; }
};


} // namespace rt3


#endif