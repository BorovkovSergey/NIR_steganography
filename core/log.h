#pragma once
#include <string>

namespace nir_log
{
void error( const std::string& message );
void info( const std::string& message );
void warning( const std::string& message );
} // namespace nir_log
