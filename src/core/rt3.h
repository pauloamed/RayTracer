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
#include "basic_types.h"

//== Alias to the chosen data structure to implement a dictionary.
//#define Dictionary std::unordered_map
#define Dictionary std::map

//=== Aliases
namespace rt3 {


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
const vector<string> mapping_t_names = {"screen", "spherical"}; 

// Background type
enum class bg_type_t { colors = 0 };
const vector<string> bg_type_t_names = {"colors"};

/// List of support image file formats.
enum class image_type_t : int { PNG=0, PPM3, PPM6 };
const vector<string> image_type_t_names = {"png", "ppm3", "ppm6"};

/// List of supported camera types
enum class camera_type_t : int { orthographic, perspective };
const vector<string> camera_type_t_names = {"orthographic", "perspective"};

/// List of supported integrator types
enum class integrator_type_t : int { flat, depth_map, normal_map };
const vector<string> integrator_type_t_names = {"flat", "depth_map", "normal_map"};

/// List of supported material types
enum class material_type_t : int { flat };
const vector<string> material_type_t_names = {"flat"};

/// List of supported object types
enum class object_type_t : int { sphere };
const vector<string> object_type_t_names = {"sphere"};

//==============

// Global Forward Declarations
class Film;
class Background;
class BackgroundColor;
class Primitive;
class Ray;
class Material;
class Shape;
class Surfel;


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
inline real_type Radians(real_type deg) { return ((real_type)M_PI / 180.f) * deg; }

/// Radians to degreees.
inline real_type Degrees(real_type rad) { return (180.f / (real_type)M_PI) * rad; }


inline Color interpolate_color(const float t, const Color &a, const Color &b){
  return Color{{
    (int) Lerp(t, a.x(), b.x()),
    (int) Lerp(t, a.y(), b.y()),
    (int) Lerp(t, a.z(), b.z()),
  }};
}

inline Color clampColor(Color x){
    return Color({
        Clamp<int, int, int>(x.at(0), 0, 255),
        Clamp<int, int, int>(x.at(1), 0, 255),
        Clamp<int, int, int>(x.at(2), 0, 255),        
    });
}

} // namespace rt3

#endif // RT3_H
