#include <stdexcept>
#ifndef RT3_H
#define RT3_H

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
using std::shared_ptr;
using std::unique_ptr;
#include <vector>
using std::vector;
#include <stack>
using std::stack;
#include <map>
using std::map;
#include <unordered_map>
using std::unordered_map;
#include <string>
using std::string;
#include <algorithm>
using std::max;
using std::min;
using std::sort;
using std::swap;
#include <utility>
using std::make_pair;
using std::pair;
#include <tuple>
using std::tuple;
#include <array>
using std::array;

#include "error.h"

//== Alias to the chosen data structure to implement a dictionary.
//#define Dictionary std::unordered_map
#define Dictionary std::map

//=== Aliases
namespace rt3 {
// wrapper class for structured point values
template <typename T, int size> class StructuredValues {
public:
  array<T, size> values;

  StructuredValues():values(array<T,size>()){};

  StructuredValues(T _x, T _y){
    if (size != 2) {
      string errorMsg = "Invalid number of arguments!\nIs " +
                        std::to_string(size) + " but should be " +
                        std::to_string(2);
      RT3_ERROR(errorMsg);
    }
    StructuredValues();
    values[0] = _x;
    values[1] = _y;
  };

  StructuredValues(T _x, T _y, T _z){
    if (size != 3) {
      string errorMsg = "Invalid number of arguments!\nIs " +
                        std::to_string(size) + " but should be " +
                        std::to_string(3);
      RT3_ERROR(errorMsg);
    }
    StructuredValues();
    values[0] = _x;
    values[1] = _y;
    values[2] = _z;
  };

  StructuredValues(const vector<T> &_values){
    assert(values.size() == size);
    std::copy_n(_values.begin(), _values.size(), values.begin());
  }

  T operator[]( const int i ) const{
      return values[i];
  }

  inline T x() const{ return values[0]; }
  inline T y() const{ return values[1]; }
  inline T z() const{ 
    assert(size == 3);
    return values[2]; 
  }

};

/// Type aliases
template <typename T, int size> using Point = StructuredValues<T, size>;
template <typename T, int size> using Vector = Point<T, size>;

// Vector/Normal3f and Point3f
using Point3f = Point<float, 3>;
using Vector3f = Vector<float, 3>;
using Normal3f = Vector<float, 3>;

// Other types
using Color = StructuredValues<float, 3>;
using ColorXYZ = Color;
// using Spectrum = StructuredValues<float, 3>;
using Ray = StructuredValues<float, 3>;

// List of points
using ListPoint3f = std::vector<Point3f>;

// Vector3i and PointNi
using Vector3i = Vector<int, 3>;
using Point3i = Point<int, 3>;
using Point2i = Point<int, 2>;
using Point2f = Point<float, 2>;

template <typename T, size_t S>
std::ostream &operator<<(std::ostream &os, const std::array<T, S> &v) {
  os << "[ ";
  for (const auto &e : v)
    os << e << " ";
  os << "]";
  return os;
}

//========= Enums
/// Types of color mapping scheme
enum class mapping_t {
  screen = 0, //!< background is assigned to the image screen
  spherical   //!< background is assigne to a sphere surrounding the scene.
};

// Background type
enum class bg_type_t { colors = 0 };

//==============

// Global Forward Declarations
class Film;
class Background;
class BackgroundColor;

//=== aliases
typedef float real_type;
typedef size_t size_type;
typedef std::tuple<bool, std::string> result_type;

/// This struct holds information provided via command line arguments
struct RunningOptions {
  RunningOptions() : filename{""}, outfile{""}, quick_render{false} {
    crop_window[0][0] = 0; //!< x0
    crop_window[0][1] = 1; //!< x1,
    crop_window[1][0] = 0; //!< y0
    crop_window[1][1] = 1; //!< y1
  }
  // [row=0] -> X; [row=1] -> Y
  // x0, x1, y0, y1
  real_type crop_window[2][2]; //!< Crop window to render. 1 = 100% of the full
                               //!< resolution.
  std::string filename;        //!< input scene file name.
  std::string outfile;         //!< output image file name.
  bool quick_render; //!< when set, render image with 1/4 of the requested
                     //!< resolution.
};

/// Lambda expression that returns a lowercase version of the input string.
auto CSTR_LOWERCASE = [](const char *t) -> std::string {
  std::string str{t};
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
};
/// Lambda expression that returns a lowercase version of the input string.
auto STR_LOWERCASE = [](const string &t) -> std::string {
  std::string str{t};
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
};

//=== Global Inline Functions

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
inline float Radians(float deg) { return ((float)M_PI / 180.f) * deg; }

/// Radians to degreees.
inline float Degrees(float rad) { return (180.f / (float)M_PI) * rad; }
} // namespace rt3

#endif // RT3_H
