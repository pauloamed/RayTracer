#include "background.h"

namespace rt3 {


    /*!
     * Samples a color base on spherical interpolation of an image ou colored background.
     *
     * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
     * \return The interpolated color.
     */
    Spectrum Background::sampleXYZ( const Point2f &pixel_ndc ) const
    {
        // TODO
        return Spectrum{0,0,0};
    }


    // Factory function pattern.
    // This is the function that retrieves from the ParamSet object
    // all the information we need to create a BackgroundColor (Background) object.
    BackgroundColor * create_color_background( const ParamSet &ps )
    {
        std::cout << ">>> Inside create_color_background()\n";
        int yres = retrieve( ps, "y_res", int(720) );

        bg_type_t background_type = retrieve( ps, "y_res", bg_type_t::colors );
        
        return new BackgroundColor();
    }
}
