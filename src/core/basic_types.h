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
using std::pair;
using std::copy_n;
using std::min;
using std::max;
using std::swap;

namespace rt3{

//=== aliases
typedef float real_type;
typedef size_t size_type;
typedef std::tuple<bool, std::string> result_type;

/*! Linear interpolation.
 * \param t The parameter, in [0,1].
 * \param v1 The initial interpolation value.
 * \param v2 The final interpolation value.
 * \return The interpolated value.
 */
//
inline float Lerp(float t, float v1, float v2) {
  return (1.f - t) * v1 + t * v2;
}

/// Clamp T to [low,high].
template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
  if (val < low)
    return low;
  else if (val > high)
    return high;
  else
    return val;
}

/// Degrees to radians.
inline real_type Radians(real_type deg) { return ((real_type)M_PI / 180.f) * deg; }

/// Radians to degreees.
inline real_type Degrees(real_type rad) { return (180.f / (real_type)M_PI) * rad; }



// wrapper class for structured point values
template <typename T, int size> class StructuredValues {
protected:
  array<T, size> values;

public:
  StructuredValues(){ values = array<T,size>();}
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

    Vector abs() const{
      Vector v(*this);
      for(int i = 0; i < size; ++i){
        v[i] = fabs(v[i]);
      }
      return v;
    }

    Vector cross(const Vector &x) const{
      Vector v(*this);
      v[0] = this->at(1) * x.at(2) - this->at(2) * x.at(1);
      v[1] = this->at(2) * x.at(0) - this->at(0) * x.at(2);
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

  Point operator*(real_type t) const{
      Point p(*this);
      for(auto &x : p.values) x *= t;
      return p;
  }

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
Point<T, size> operator+(const Point<T, size> x, const Point<T,size> y){
    Point<T, size> v(x);
    for(int i = 0; i < size; ++i){
        v[i] += y.at(i);
    }
    // std::cout << "+ " << x.toString() << " " << y.toString() << " " << v.toString() << std::endl;
    return v;
}


template<typename T, int size>
Vector<T, size> operator+(const Vector<T, size> &x, const Vector<T,size> &y){
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

// List of points
using ListPoint3f = std::vector<Point3f>;

// Other types

class Color : public StructuredValues<real_type, 3>{
public:

  Color():StructuredValues<real_type,3>(){}

  Color(const vector<real_type> &v):StructuredValues<real_type, 3>(v){}

  Color clamp(){
    Color newColor(*this);
    for(auto &x : newColor.values){
      x = Clamp<real_type, real_type, real_type>(x, 0, 1);
    }
    return newColor;
  }

  static inline Color interpolate_color(const float t, const Color &a, const Color &b){
    return Color{{
      (real_type) Lerp(t, a.x(), b.x()),
      (real_type) Lerp(t, a.y(), b.y()),
      (real_type) Lerp(t, a.z(), b.z()),
    }};
  }

  static inline Color make_color_from_real(const vector<real_type> &v){
    return Color(v);
  }

  static inline Color make_color_from_int(vector<real_type> v){
    for(auto &x : v) x /= 255;
    return make_color_from_real(v);
  }
};

Color operator+(const Color &x, const Color &y);

Color operator*(const Color &x, const Color &y);

Color operator*(const Color &x, real_type y);

const Color BLACK = Color({0, 0, 0});

class ColorInt : public StructuredValues<int, 3>{
public:

  ColorInt():StructuredValues<int,3>(){}

  ColorInt(const Color &c):ColorInt(){
    for(int i = 0; i < 3; ++i){
      values[i] = c.at(i) * 255;
    }
  }
};





class Ray {
public:
    Ray (const Point3f& _o, const Vector3f& _d ) : o{_o}, d{_d.normalize()} {}
    Point3f o; //!< origin
    Vector3f d; //!< direction
    Point3f operator()(real_type t) const { 
      return o + d * t; 
    }
};


struct ScreenWindow{
    real_type left, right;
    real_type bottom, top;

    ScreenWindow() = default;

    ScreenWindow(real_type l, real_type r, real_type b, real_type t):
    left(l), right(r), bottom(b), top(t){}

    ScreenWindow(const vector<real_type> &vals):
      ScreenWindow(vals[0], vals[1], vals[2], vals[3]){}

    real_type width() const { return right - left; }
    real_type height() const { return top - bottom; }
};



template<typename T>
T fastExp(T x, int exp){
    T ans = T(1);
    while(exp > 0){
        if(exp%2) ans = ans * x; 
        exp >>= 1;
        x = x * x;
    }
    return ans;
}


} // namespace rt3


#endif