#ifndef PARSER_H
#define PARSER_H 1

/*!
 * Implementation of XML processing functions.
 * @file parser.cpp
 */

#include <tinyxml2.h>
#include <iostream>
using std::endl;
using std::clog;
using std::cerr;
using std::cout;
using std::boolalpha;
#include <iomanip>
using std::setw;
#include <string>
using std::string;
#include <memory>
using std::make_unique;
using std::make_shared;
using std::shared_ptr;
using std::unique_ptr;
#include <algorithm>
using std::copy;
#include <sstream>
using std::stringstream;
#include <iterator>
using std::istream_iterator;
using std::begin;
using std::endl;
#include <vector>
using std::vector;
#include <utility>
using std::pair;
#include <optional>
using std::optional;

#include "paramset.h"
#include "error.h"

namespace rt3 {

    /// Lambda expression that transform a c-style string to a lowercase c++-stype string version.
    auto STR_LOWER = []( const char * c_str )->std::string
    {
        std::string str{ c_str };
        std::transform( str.begin(), str.end(), str.begin(), ::tolower );
        return str;
    };

    // === Enumerations
    /// Type of possible paramter types we may read from the input scene file.
    enum class param_type_e : int { BOOL=0,
        INT,           //!< Single integet
        UINT,          //!< Single unsigned int
        REAL,          //!< Single real number
        VEC3F,         //!< Single Vector3f
        VEC3I,         //!< Single Vector3i
        NORMAL3F,      //!< Single Normal3f
        POINT3F,       //!< Single Point3f
        POINT2I,       //!< Single Point2i
        COLOR,         //!< Single Color
        SPECTRUM,      //!< Single Spectrum
        STRING,        //!< Single string
        ARR_INT,       //!< An array of integers
        ARR_REAL,      //!< An array of real numbers
        ARR_VEC3F,     //!< An array of Vector3f
        ARR_VEC3I,     //!< An array of Vector3i
        ARR_POINT3F,   //!< An array of Point3f
        ARR_COLOR,     //!< An array of Color
        ARR_NORMAL3F   //!< An array of Normal3f
    };
    // === parsing functions.
    void parse( const char* );
    void parse_tags(  tinyxml2::XMLElement *, int );
    void parse_parameters( tinyxml2::XMLElement *p_element, const vector<std::pair<param_type_e, string>>param_list, ParamSet *ps_out );

    //-------------------------------------------------------------------------------
} // namespace RT3

#endif // PARSER_H
