#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <iomanip> // setw()
#include <string>

// Will retrieve (implicitly) file name and line number
#define SC SourceContext(__FILE__, __LINE__) 

#define RT3_ERROR( msg ) Error( msg, SC )

#define RT3_WARNING( msg ) Warning( msg, SC )

#define RT3_MESSAGE( msg ) Message( msg )

namespace  rt3 {
    // Holds context information for a warning or error while pre-processing scene file or executing the renderer.
    // Use macro SC to create one
    // Struct for keeping info about file and line number in which the log occoured

    const int LOG_PADDING = 80;

    struct SourceContext {
        const char* file; // File in which log occoured
        int line; // Line number log

        // Default init
        SourceContext(const char* file="n/a", int line=0) : file(file), line(line) { }
        
        // Default copy assign
        SourceContext& operator=( const SourceContext& ) = default;
        
        // Default copy constructor
        SourceContext( const SourceContext& clone) = default;
        
        // Default destructor
        ~SourceContext() = default;
    };

    /// Prints out the error message and exits the program.
    void Error( const std::string&,  const SourceContext& );
    /// Prints out the warning, but the program keeps running.
    void Warning( const std::string&,  const SourceContext& );
    /// Prints out a simple message, program keeps running.
    void Message( const std::string& );
} // namespace rt3

#endif
//-------------------------------------------------------------------------------
