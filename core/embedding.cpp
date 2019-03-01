#include "embedding.h"
#include "log.h"

#include <iostream>
#include <utility>
nir_embedding::nir_embedding( const std::vector<std::vector<float> >& phase ) : area_positions_( 22 ), phase_( phase ), average_count_( 0 ), clear_sequence_count_( -1 )
{
     nir_log::info( "Start nir_embedding constructor" );

     create_area_positions();
     get_average_count();
     nir_log::info( "End nir_embedding constructor" );
}

void nir_embedding::get_average_count()
{
     nir_log::info( "Start get_average_count" );
     for( auto it : area_positions_ )
     {
          average_count_ += phase_[ it.second ][ it.first ] < 0 ? -phase_[ it.second ][ it.first ] : phase_[ it.second ][ it.first ];
     }
     average_count_ /= area_positions_.size();
     nir_log::info( "End get_average_count" );
}

std::vector<float> nir_embedding::count_auxiliary_sequence( const float& area_width ) const
{
     nir_log::info( "Start count_auxiliary_sequence" );
     const float f0 = -M_PI_2;
     const float f1 = M_PI_2;
     std::vector<float> ret;
     for( size_t i = 0; i < area_positions_.size(); ++i )
     {
          if( f0 - area_width < phase_[ area_positions_[ i ].second ][ area_positions_[ i ].first ] && f0 + area_width > phase_[ area_positions_[ i ].second ][ area_positions_[ i ].first ] )
          {
               ret.push_back( 0 );
               continue;
          }
          if( f1 - area_width < phase_[ area_positions_[ i ].second ][ area_positions_[ i ].first ] && f1 + area_width > phase_[ area_positions_[ i ].second ][ area_positions_[ i ].first ] )
          {
               ret.push_back( 1 );
               continue;
          }
          ret.push_back( -1 );
     }
     nir_log::info( "End count_auxiliary_sequence" );
     return ret;
}

std::vector<std::vector<float> > nir_embedding::find_clear_sequence( const std::vector<float>& auxiliary_sequence )
{
     nir_log::info( "Start find_clear_sequence" );
     clear_sequence_count_ = 0;
     std::vector<std::vector<float> > ret;
     int counter = -1;
     bool plus = false;
     bool minus = false;
     for( size_t i = 0; i < auxiliary_sequence.size(); i++ )
     {
          if( !minus && -1 == auxiliary_sequence[ i ] )
          {
               minus = true;
               ++clear_sequence_count_;
               ret.push_back( std::vector<float>() );
               plus = false;
               ++counter;
          }
          if( !plus && 0 <= auxiliary_sequence[ i ] )
          {
               plus = true;
               ret.push_back( std::vector<float>() );
               minus = false;
               ++counter;
          }
          ret[ counter ].push_back( auxiliary_sequence[ i ] );
     }
     nir_log::info( "End find_clear_sequence" );
     return ret;
}

int nir_embedding::calculate_overlay_options( const std::vector<std::vector<float> >& clear_sequence ) const
{
     nir_log::info( "Start calculate_overlay_options" );
     int ret = 1;
     for( auto it : clear_sequence )
     {
          if( it[ 0 ] < 0 )
          {
               ret *= it.size() + 1;
          }
     }
     nir_log::info( "End calculate_overlay_options" );
     return ret;
}

// float calculate_quality_characteristics( const std::vector<float>& M, const std::vector<float>& F ) const
// {
//      nir_log::info( "Start calculate_quality_characteristics" );
//      for( size_t i = 0; i < F.size() && i < M.size(); i++ )
//      nir_log::info( "End calculate_quality_characteristics" );
//      return ret;
// }
void nir_embedding::create_area_positions()
{
     nir_log::info( "Start create_area_positions" );
     area_positions_[ 0 ].first = 3;
     area_positions_[ 0 ].second = 1;
     area_positions_[ 1 ].first = 4;
     area_positions_[ 1 ].second = 1;
     area_positions_[ 2 ].first = 2;
     area_positions_[ 2 ].second = 2;
     area_positions_[ 3 ].first = 3;
     area_positions_[ 3 ].second = 2;
     area_positions_[ 4 ].first = 4;
     area_positions_[ 4 ].second = 2;
     area_positions_[ 5 ].first = 1;
     area_positions_[ 5 ].second = 3;
     area_positions_[ 6 ].first = 2;
     area_positions_[ 6 ].second = 3;
     area_positions_[ 7 ].first = 3;
     area_positions_[ 7 ].second = 3;
     area_positions_[ 8 ].first = 4;
     area_positions_[ 8 ].second = 3;
     area_positions_[ 9 ].first = 1;
     area_positions_[ 9 ].second = 4;
     area_positions_[ 10 ].first = 2;
     area_positions_[ 10 ].second = 4;
     area_positions_[ 11 ].first = 3;
     area_positions_[ 11 ].second = 4;
     area_positions_[ 12 ].first = 4;
     area_positions_[ 12 ].second = 4;
     area_positions_[ 13 ].first = 1;
     area_positions_[ 13 ].second = 5;
     area_positions_[ 14 ].first = 2;
     area_positions_[ 14 ].second = 5;
     area_positions_[ 15 ].first = 3;
     area_positions_[ 15 ].second = 5;
     area_positions_[ 16 ].first = 1;
     area_positions_[ 16 ].second = 6;
     area_positions_[ 17 ].first = 2;
     area_positions_[ 17 ].second = 6;
     area_positions_[ 18 ].first = 3;
     area_positions_[ 18 ].second = 6;
     area_positions_[ 19 ].first = 1;
     area_positions_[ 19 ].second = 7;
     area_positions_[ 20 ].first = 2;
     area_positions_[ 20 ].second = 7;
     area_positions_[ 21 ].first = 3;
     area_positions_[ 21 ].second = 7;
     nir_log::info( "End create_area_positions" );
}
