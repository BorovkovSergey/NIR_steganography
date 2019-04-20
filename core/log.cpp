#include <iostream>
#include <fstream>

#include "log.h"

#define RST "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"

#define FRED( x ) KRED x RST
#define FGRN( x ) KGRN x RST
#define FYEL( x ) KYEL x RST

namespace
{
void print_file( const std::string& message )
{
     std::ofstream out( "../main.log", std::ios::app );
     if( out.is_open() )
     {
          out << message << std::endl;
     }
     out.close();
}
} // namespace
namespace nir_log
{
void error( const std::string& message )
{
     time_t t;
     time( &t );
     std::cout << FRED( "Error.....: " ) << message << std::endl
               << ctime( &t ) << std::endl;
     print_file( "Error: " + message + "\n" + ctime( &t ) );
}
void warning( const std::string& message )
{
     time_t t;
     time( &t );
     std::cout << FYEL( "Warning...: " ) << message << std::endl
               << ctime( &t ) << std::endl;
     //print_file( "Warning: " + message + "\n" + ctime( &t ) );
}
void info( const std::string& message )
{
 //    std::cout << FGRN( "Info......: " ) << message << std::endl;
}
} // namespace nir_log
