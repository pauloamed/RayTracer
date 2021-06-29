#ifndef FILM_H
#define FILM_H

#include "rt3.h"
#include "error.h"
#include "paramset.h"

namespace rt3 {

    /// Represents an image generated by the ray tracer.
    class Film {
        struct ColorBuffer{
            
            vector<vector<Color>> data;
            int height, width;


            ColorBuffer(int _height, int _width): height(_height), width(_width){
                data = vector<vector<Color>>(height, vector<Color>(width));
            }

            Color& get(Point2i coord){
                return data[coord[0]][coord[1]];
            }

            unsigned char* getBlob(){
                unsigned char * byte_data = new unsigned char[width * height * 3];
                int currPos = 0;
                for(auto &row : data){
                    for(auto &c : row){
                        for(int i = 0; i < 3; ++i){
                            byte_data[currPos++] = c[i];
                        }
                    }
                }
                return byte_data;
            }
        };

        public:

            /// List of support image file formats.
            enum class image_type_e : int { PNG=0, PPM3, PPM6 };

            //=== Film Public Methods
            Film( const Point2i &resolution, const std::string &filename, image_type_e imgt );
            virtual ~Film();
            
            /// Retrieve original Film resolution.
            Point2i get_resolution() const { return m_full_resolution; };
            /// Takes a sample `p` and its radiance `L` and updates the image.
            void add_sample( const Point2i &, const ColorXYZ & );
            void write_image() const;

            //=== Film Public Data
            const Point2i m_full_resolution;    //!< The image's full resolution values.
            std::string m_filename;       //!< Full path file name + extension.
            image_type_e image_type; //!< Image type, PNG, PPM3, PPM6.
            

            // Create the matrix (or vector) that will hold the image data.
            std::unique_ptr< ColorBuffer > m_color_buffer_ptr; //!< Reference to the color buffer (image) object.
            
            int height() const { return m_full_resolution[1]; }
            int width() const { return m_full_resolution[0]; } 
            

    };

    // Factory pattern. It's not part of this class.
    Film * create_film( const ParamSet &ps );
};  // namespace rt3

#endif  // FILM_H
