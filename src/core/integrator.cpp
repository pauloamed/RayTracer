#include "integrator.h"

namespace rt3{

void Integrator::render( unique_ptr<Film> &film ) {
    // Perform objects initialization here.
    // The Film object holds the memory for the image.
    // ...
    auto w = film.width(); // Retrieve the image dimensions in pixels.
    auto h = film.height();
    // Traverse all pixels to shoot rays from.
    for ( int j = 0 ; j < ; j++ ) {
        for( int i = 0 ; i < w ; i++ ) {
            // Not shooting rays just yet; so let us sample the background.
            auto color = background.sample( float(i)/float(w), float(j)/float(h) ); // get background color.
            film.add( Point2{i,j}, color ); // set image buffer at position (i,j), accordingly.
        }
    }
    // send image color buffer to the output file.
    film.write_image();

}

} // namespace rt3

