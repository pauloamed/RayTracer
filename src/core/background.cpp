#include "background.h"

namespace rt3 {
/*!
 * Samples a color base on spherical interpolation of an image or colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */

Color interpolate_color(const float t, const Color &a, const Color &b){
  return Color{
    Lerp(t, a.x(), b.x()),
    Lerp(t, a.y(), b.y()),
    Lerp(t, a.z(), b.z()),
  };
}

Color BackgroundColor::sampleXYZ(const Point2f &pixel_ndc) const {
  float t_hor = pixel_ndc.x();
  float t_ver = pixel_ndc.y();
  Color top = interpolate_color(t_hor, corners[Corners_e::tl], corners[Corners_e::tr]);
  Color bottom = interpolate_color(t_hor, corners[Corners_e::bl], corners[Corners_e::br]);

  return interpolate_color(t_ver, top, bottom);
}

BackgroundColor *create_color_background(const ParamSet &ps) {
  Color tl = retrieve( ps, "tl", Color());
  Color bl = retrieve( ps, "bl", Color());
  Color tr = retrieve( ps, "tr", Color());
  Color br = retrieve( ps, "br", Color());

  auto mtype = retrieve( ps, "mapping", string("screen"));
  
  mapping_t mt = mapping_t::screen;
  if(mtype == "spherical") mt = mapping_t::spherical;
  return new BackgroundColor(mt, bl, tl, tr, br);
}
} // namespace rt3
