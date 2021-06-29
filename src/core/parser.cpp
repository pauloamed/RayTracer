/*!
 * Implementation of XML processing functions.
 * @file parser.h
 */

#include "paramset.h"
#include "parser.h"
#include "api.h"
#include "rt3.h"

// === Function Implementation

namespace rt3 {

using rt3::Point3f;
using rt3::Vector3f;
using rt3::Vector3i;

template<typename T>
T getPrimValue(stringstream &ss){
    if constexpr(std::is_same_v<T, bool>) {
        string boolean;
        ss >> boolean;
        if(boolean == "true") {
            return true;
        } else if(boolean == "false") {
            return false;
        } else {
            // TODO
        }
    } else {
        T ret;
        ss >> ret;
        return ret;
    }
}


template<typename T>
vector<T> getMultipleValues(stringstream &ss, int size = -1){
    vector<T> values;
    if(size == -1){
        while(ss.good()) values.push_back(getPrimValue<T>(ss));
    } else {
        values.resize(size);
        for(auto &x : values) x = getPrimValue<T>(ss);
    }
    return values;
}


/// ...
template<typename T> 
void parse_single_prim_attrib( tinyxml2::XMLElement * p_element,
        ParamSet *ps_out, const string &name ){
    
    const char *attr_val = p_element->Attribute(name.c_str());
    auto str = string(attr_val);
    stringstream ss(str);

    (*ps_out)[name] = make_shared<Value<T>>( Value<T>(getPrimValue<T>(ss)));
    delete attr_val;
}


template<typename T_INTERNAL, typename T, int size> 
void parse_single_composite_attrib( tinyxml2::XMLElement * p_element,
        ParamSet *ps_out, const string &name ){
    
    const char *attr_val = p_element->Attribute(name.c_str());
    auto str = string(attr_val);
    stringstream ss(str);

    vector<T_INTERNAL> values = getMultipleValues<T_INTERNAL>(ss, size);
    
    (*ps_out)[name] = make_shared<Value<T>>( Value<T>(T()) ); // T(values)
    delete attr_val;
}


template<typename T, typename T_INTERNAL, int internal_size> 
void parse_array_composite_attrib( tinyxml2::XMLElement * p_element,
        ParamSet *ps_out, const string &name ){
    
    const char *attr_val = p_element->Attribute(name.c_str());
    auto str = string(attr_val);
    stringstream ss(str);

    vector<T> values;
    while(ss.good()){
        values.push_back(T()); // getMultipleValues<T_INTERNAL>(ss, internal_size)
    }
    
    (*ps_out)[name] = make_shared<Value<vector<T>>>(
        Value<vector<T>>(vector<T>(values))
    );
    delete attr_val;
}


template<typename T> 
void parse_array_prim_attrib( tinyxml2::XMLElement * p_element,
        ParamSet *ps_out, const string &name ){
    
    const char *attr_val = p_element->Attribute(name.c_str());
    auto str = string(attr_val);
    stringstream ss(str);

    vector<T> values = getMultipleValues<T>(ss);
    
    (*ps_out)[name] = make_shared<Value<vector<T>>>(
        Value<vector<T>>(vector<T>(values))
    );
    delete attr_val;
}


/// This is the entry function for the parsing process.
void parse( const char* scene_file_name )
{
    tinyxml2::XMLDocument xml_doc;

    // Load file.
    if ( xml_doc.LoadFile( scene_file_name ) != tinyxml2::XML_SUCCESS )
        RT3_ERROR( std::string{"The file \""} + scene_file_name + std::string{"\" either is not available OR contains an invalid RT3 scene provided!"} );

    // ===============================================
    // Get a pointer to the document's root node.
    tinyxml2::XMLNode * p_root = xml_doc.FirstChild();
    if ( p_root == nullptr )
        RT3_ERROR( "Error while trying to find \"RT3\" tag in the scene file." );
    // ===============================================

    // Get the first-level tag inside the root node.
    auto *p_child { p_root->FirstChildElement( ) };
    if ( p_child == nullptr)
        RT3_ERROR( "No \"children\" tags found inside the \"RT3\" tag. Empty scene file?" );

    parse_tags( p_child, /* initial level */ 0 );
}

/// Main loop that handles each possible tag we may find in a RT3 scene file.
void parse_tags(  tinyxml2::XMLElement *p_element, int level )
{
    // All messages sent to std::clog is for DEBUG purposes.
    std::clog << "[parse_tags()]: level is " << level << std::endl;

    // Traverse all items on the children's level. 
    while ( p_element != nullptr )
    {
        // Convert the attribute name to lowecase before testing it.
        auto tag_name = CSTR_LOWERCASE( p_element->Value() );
        clog << "\n"<< setw(level*3) << "" << "***** Tag id is `" << tag_name << "`, at level " << level << std::endl;

        // Big switch for each possible RT3 tag type.
        if ( tag_name == "background" )
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING  , "type"        },
                { param_type_e::STRING  , "filename"    }, // Texture file name.
                { param_type_e::STRING  , "mapping"     }, // Type of mapping required.
                { param_type_e::COLOR   , "color"       }, // Single color for the entire background.
                { param_type_e::COLOR   , "tl"          }, // Top-left corner
                { param_type_e::COLOR   , "tr"          }, // Top-right corner
                { param_type_e::COLOR   , "bl"          }, // Bottom-left corner
                { param_type_e::COLOR   , "br"          }  // Bottom-right corner
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            // Calling the corresponding API method.
            API::background( ps );
        }
        else if ( tag_name == "film" )
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING   , "type"        },
                { param_type_e::STRING   , "filename"    },
                { param_type_e::STRING   , "img_type"    },
                { param_type_e::INT      , "x_res"       },
                { param_type_e::INT      , "y_res"       },
                { param_type_e::ARR_REAL , "crop_window" },
                { param_type_e::STRING   , "gamma_corrected" } // bool
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            // Calling the corresponding API method.
            API::film( ps );
        }
        else if ( tag_name == "world_begin" )
        {
            //std::clog << ">>> Entering WorldBegin, at level " << level+1 << std::endl;
            // We should get only one `world` tag per scene file.
            API::world_begin();
        }
        else if ( tag_name == "world_end" )
        {
            API::world_end();
            //std::clog << ">>> Leaving WorldBegin, at level " << level+1 << std::endl;
        }
        //else RT3_WARNING( "Undefined tag `" + tag_name + "` found!" );

        // Get next (to the right) sibling on this tree level.
        p_element = p_element->NextSiblingElement();
    }
}

/// Universal parameters parser.
/*!
 * This function receives a list of pairs <param_type, name>, traverse all the attributes found
 * in `p_element` and extract the attribute values into the `ps_out` `ParamSet` object.
 * Only named attributes found are read into the `ps_out`.
 *
 * @param p_element XML element we are extracting information from.
 * @param param_list List of pairs <param_type, name> we need to extract from the XML element.
 * @param ps_out The `ParamSet` object we need to fill in with parameter information extracted from the XML element.
 */
void parse_parameters( tinyxml2::XMLElement * p_element,
        const vector<std::pair<param_type_e, string>> param_list, ParamSet *ps_out )
{
    //std::clog << "parse_parameters(): p_element = " << p_element << endl;

    // Traverse the list of paramters pairs: type + name.
    for ( const auto& e : param_list )
    {
        const auto & [ type, name ] = e; // structured binding, requires C++ 17
        std::clog << "---Parsing att \"" << name << "\", type = " << (int)type << "\n";

        // This is just a dispatcher to the proper extraction functions.
        switch ( type )
        {
            case param_type_e::BOOL:
                parse_single_prim_attrib<bool>( p_element, ps_out, name );
                break;
            case param_type_e::UINT:
                parse_single_prim_attrib<uint>( p_element, ps_out, name );
                break;
            case param_type_e::INT:
                parse_single_prim_attrib<int>( p_element, ps_out, name );
                break;
            case param_type_e::REAL:
                parse_single_prim_attrib<real_type>( p_element, ps_out, name );
                break;
            case param_type_e::STRING:
                parse_single_prim_attrib<string>( p_element, ps_out, name );
                break;
            case param_type_e::VEC3F:
                parse_single_composite_attrib<int, Vector3f, int(3)>( p_element, ps_out, name );
                break;
            case param_type_e::VEC3I:
                parse_single_composite_attrib<int, Vector3i, int(3)>( p_element, ps_out, name );
                break;
            case param_type_e::NORMAL3F:
                parse_single_composite_attrib<int, Normal3f, int(3)>( p_element, ps_out, name );
                break;
            case param_type_e::POINT3F:
                parse_single_composite_attrib<int, Point3f, int(3)>( p_element, ps_out, name );
                break;
            case param_type_e::POINT2I:
                parse_single_composite_attrib<int, Point2i, int(2)>( p_element, ps_out, name );
                break;
            case param_type_e::COLOR:
                parse_single_composite_attrib<int, ColorXYZ, int(3)>( p_element, ps_out, name );
                break;
            case param_type_e::SPECTRUM:
                parse_single_composite_attrib<float, Spectrum, int(3)>( p_element, ps_out, name );
                break;
            case param_type_e::ARR_REAL:
                parse_array_prim_attrib<real_type>( p_element, ps_out, name );
                break;
            case param_type_e::ARR_INT:
                parse_array_prim_attrib<int>( p_element, ps_out, name );
                break;
            case param_type_e::ARR_VEC3F:
                parse_array_composite_attrib<Vector3f, float, 3>( p_element, ps_out, name );
                break;
            case param_type_e::ARR_VEC3I:
                parse_array_composite_attrib<Vector3i, int, 3>( p_element, ps_out, name );
                break;
            case param_type_e::ARR_NORMAL3F:
                parse_array_composite_attrib<Normal3f, float, 3>( p_element, ps_out, name );
                break;
            case param_type_e::ARR_POINT3F:
                parse_array_composite_attrib<Point3f, float, 3>( p_element, ps_out, name );
                break;
            case param_type_e::ARR_COLOR:
                parse_array_composite_attrib<ColorXYZ, float, 3>( p_element, ps_out, name );
                break;
            default:
                RT3_WARNING( string{"parse_params(): unkonwn param type received!" } );
                break;
        }
        clog << "---Done!\n";
    }
}


//-------------------------------------------------------------------------------

} // namespace rt3
