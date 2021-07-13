#include "integrator.h"

namespace rt3{

void Integrator::render( const unique_ptr<Scene> &scene ) {
    // Perform objects initialization here.
    // The Film object holds the memory for the image.
    // ...
    auto w = camera->film->width(); // Retrieve the image dimensions in pixels.
    auto h = camera->film->height();
    // Traverse all pixels to shoot rays from.
    for ( int i = 0 ; i < h; i++ ) {
        for( int j = 0 ; j < w ; j++ ) {
            
            Ray ray = camera->generate_ray( i, j );
            auto color = scene->background->sampleXYZ( Point2f{{float(i)/float(h), float(j)/float(w)}} ); // get background color.
            
            if(scene->primitive->intersect_p(ray)){
                color = ColorXYZ({255,0,0});  // Just paint it red.
            }
         
            camera->film->add_sample( Point2i{{i,j}}, color ); // set image buffer at position (i,j), accordingly.
        }
    }
    // send image color buffer to the output file.
    camera->film->write_image();

}

} // namespace rt3

