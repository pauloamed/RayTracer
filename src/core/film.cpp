#include "film.h"
#include "paramset.h"
#include "image_io.h"
#include "api.h"

#include <cmath>

namespace rt3 {

    //=== Film Method Definitions
    Film::Film( const Point2i &resolution, const std::string &filename , image_type_e imgt ) :
        m_full_resolution{resolution},
        m_filename{filename},
        image_type{ imgt }
    {
        // m_color_buffer_ptr = make_unique<vector<vector<Color>>>(vector<vector<Color>>(resolution[0], vector<Color>(resolution[1])));
        m_color_buffer_ptr = make_unique<ColorBuffer>(new ColorBuffer[resolution[0] * resolution[1]]);
    }

    Film::~Film()
    {
    }

    /// Add the color to image.
    void Film::add_sample ( const Point2i &pixel_coord, const ColorXYZ &pixel_color )
    {
        m_color_buffer_ptr[(int) pixel_color[0] * pixel_coord[1]] = pixel_color;
    }

    /// Convert image to RGB, compute final pixel values, write image.
    void Film::write_image(void) const
    {
        bool result = false;
        if(image_type == image_type_e::PPM3)
        {
            result = save_ppm3( m_color_buffer_ptr.get(), width(), height(), 3,  m_filename);
        } else if(image_type == image_type_e::PPM6)
        {
            result = save_ppm6( m_color_buffer_ptr.get(), width(), height(), 3,  m_filename);
        } else if(image_type == image_type_e::PNG)
        {
            result = save_png( m_color_buffer_ptr.get(), width(), height(), 3,  m_filename);
        }

        if(!result) RT3_ERROR("Failed to save image.");
    }
    
    // Factory function pattern.
    // This is the function that retrieves from the ParamSet object
    // all the information we need to create a Film object.
    Film * create_film( const ParamSet &ps )
    {
        std::cout << ">>> Inside create_film()\n";
        std::string filename;
        // Let us check whether user has provided an output file name via
        // command line arguments in main().
        if ( API::curr_run_opt.outfile != "")
        {
            // Retrieve filename defined via CLI.
            filename = API::curr_run_opt.outfile;
            // Retrieve another filename, now from the ParamSet object.
            std::string filename_from_file = retrieve( ps, "filename", std::string{""} );
            if ( filename_from_file != "") // We also get a filename from scene file...
                RT3_WARNING( string{"Output filename supplied on command line, \""} +
                         API::curr_run_opt.outfile +
                         string{"\" is overriding filename provided in scene description file, \""} +
                         filename_from_file );
        }
        else
        {
            // Try yo retrieve filename from scene file.
            filename = retrieve( ps, "filename", std::string{"image.png"} );
        }

        // Read resolution.
        // Aux function that retrieves info from the ParamSet.
        int xres = retrieve( ps, "x_res", int(1280) );
        // Aux function that retrieves info from the ParamSet.
        int yres = retrieve( ps, "y_res", int(720) );
        // Quick render?
        if ( API::curr_run_opt.quick_render == true)
        {
            // decrease resolution.
            xres = std::max(1, xres / 4);
            yres = std::max(1, yres / 4);
        }


        // TODO
        // Read crop window information.

        // Note that the image type is fixed here. Must be read from ParamSet, though.
        return new Film( Point2i{ xres, yres}, filename, Film::image_type_e::PNG );
    }
}  // namespace pbrt
