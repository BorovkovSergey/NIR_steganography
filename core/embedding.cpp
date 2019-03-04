#include "embedding.h"
#include "log.h"
#include "misc.h"
#include "dft.h"

#include <iostream>
#include <utility>

namespace
{
int calculate_replacements( const std::vector<float>& prev_vec, const std::vector<float>& buf )
{
     int ret = 0;
     if( prev_vec.size() != buf.size() )
     {
          std::cout << "WARNING: prev_vec.size() != buf.size()"; // todo fixme
     }
     for( size_t i = 0; i < prev_vec.size() && i < buf.size(); ++i )
     {
          if( prev_vec[ i ] != buf[ i ] )
          {
               ++ret;
          }
     }
     return ret;
}
void next_step( const std::vector<float>& m_vec, std::vector<float>& buf )
{
     for( size_t i = buf.size() - 1; i >= 0; --i )
     {
          if( -1 != buf[ i ] )
          {
               buf[ i ] = -1;
               for( size_t j = i + 1; j < buf.size(); ++j )
               {
                    std::vector<float>::iterator it = buf.begin() + j;
                    buf[ j ] = m_vec[ j - std::count( buf.begin(), it, -1 ) ];
               }
               break;
          }
     }
}

std::vector<float> count_new_auxiliary_sequence( const std::vector<std::vector<float> >& inverseTransform )
{
     nir_dft img = nir_dft( inverseTransform );
     std::cout << "1111111111111111111111111111111111111111111" << std::endl;
     nir_misc::print_vec( img.get_img() );

     nir_embedding emb = nir_embedding( img.get_img(), img.calc_phase( img.im(), img.re() ), img.calc_amp( img.im(), img.re() ), img.im(), img.re() );
     std::vector<float> test = emb.count_auxiliary_sequence( 1 ); // todo fixme 1
     nir_misc::printv( test );
     //todo fixme
     return test;
}
} // namespace
nir_embedding::nir_embedding( const std::vector<std::vector<float> >& img, const std::vector<std::vector<float> >& phase,
                              const std::vector<std::vector<float> >& ampl, const std::vector<std::vector<float> >& im, std::vector<std::vector<float> > re )
     : area_positions_( 22 ), phase_( phase ),
     average_count_( 0 ),
     clear_sequence_count_( -1 ), img_( img ),
     ampl_( ampl ),
     im_( im ),
     re_( re )
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

float nir_embedding::calculate_quality_characteristics( const std::vector<float>& M, const std::vector<float>& F ) const
{
     nir_log::info( "Start calculate_quality_characteristics" );
     float s = 0;
     int extra_i = 0;
     float d = 0;
     for( size_t i = 0; i + extra_i < F.size() && i < M.size(); ++i )
     {
          if( -1 == F[ i + extra_i ] )
          {
               ++extra_i;
               --i;
               continue;
          }
          if( F[ i + extra_i ] == M[ i ] )
          {
               ++s;
               continue;
          }
          ++d;
     }
     nir_log::info( "End calculate_quality_characteristics" );
     return ( s - d ) / ( s + d );
}

std::vector<float> nir_embedding::find_best_embedding( const std::vector<float>& prev_vec, const std::vector<float>& m_vec )
{
     nir_log::info( "Start find_best_embedding" );
     std::vector<float> buf( m_vec );
     while( prev_vec.size() > buf.size() )
     {
          buf.push_back( -1 );
     }
     std::vector<float> ret;
     int capacity = INT_MAX;
     int replacements_counter = INT_MAX;
     int buf_replacements_counter = 0;
     int buf_capacity = 0;
     while( buf.size() - 1 >= std::count( buf.begin(), buf.end(), -1 ) )
     {
          buf_capacity = ret.size() - std::count( ret.begin(), ret.end(), -1 );
          buf_replacements_counter = calculate_replacements( prev_vec, buf );
          if( replacements_counter > buf_replacements_counter ||
              ( replacements_counter >= buf_replacements_counter && capacity < buf_capacity ) )
          {
               replacements_counter = buf_replacements_counter;
               capacity = buf_capacity;
               ret = buf;
          }
          next_step( m_vec, buf );
     }
     buf_replacements_counter = calculate_replacements( prev_vec, buf );
     if( replacements_counter > buf_replacements_counter )
     {
          replacements_counter = buf_replacements_counter;
          buf_replacements_counter = 0;
          ret = buf;
     }
     std::cout << "replacements_counter = " << replacements_counter << std::endl
               << "capacity = " << capacity << std::endl;
     nir_log::info( "End find_best_embedding" );
     return ret;
}

std::vector<std::vector<float> > nir_embedding::embedded_new_values_in_phase_matrix( const std::vector<float>& new_values, std::vector<std::vector<float> > img )
{
     for( size_t i = 0; i < area_positions_.size(); ++i )
     {
          img[ area_positions_[ i ].second ][ area_positions_[ i ].first ] = new_values[ i ];
     }
     return img;
}
//std::vector<float> nir_embedding::do_test_embedded( const std::vector<float>& prev, const std::vector<float>& input, int a, int b )

std::vector<float> nir_embedding::do_test_embedded( const std::vector<float>& prev, const std::vector<float>& input )
{
     nir_log::info( "Start do_test_embedded" );

     std::vector<float> ret;
     if( prev.size() != input.size() )
     {
          nir_log::warning( "prev.size() != input.size() in function do_test_embedded" );
     }
     for( size_t i = 0; i < prev.size() && i < input.size(); ++i )
     {
        //   if( prev[ i ] == input[ i ] )
        //   {
        //        ret.push_back( phase_[ area_positions_[ i ].second ][ area_positions_[ i ].first ] );
        //        continue;
        //   }
          if( input[ i ] == 0 )
          {
               ret.push_back( -M_PI_2 );
               continue;
          }
          if( input[ i ] == 1 )
          {
               ret.push_back( M_PI_2 );
               continue;
          }
          ret.push_back( phase_[ area_positions_[ i ].second ][ area_positions_[ i ].first ]);//[ i % 7 ][ 3 + i % 3 ] );
     }
     std::cout << "ret" << std::endl;
     nir_misc::printv( ret );
     cv::Mat embedded_img;
     nir_misc::print_vec( embedded_new_values_in_phase_matrix( ret, phase_ ) );

     nir_misc::vecToMat( nir_dft::do_dft( embedded_new_values_in_phase_matrix( ret, phase_ ), ampl_, im_ ), re_, embedded_img );
     cv::Mat inverseTransform;
     cv::dft( embedded_img, inverseTransform, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT );
     std::cout << "new dft " << std::endl;
     nir_misc::print_vec( nir_dft::do_dft( embedded_new_values_in_phase_matrix( ret, phase_ ), ampl_, im_ ) );
     nir_log::info( "End do_test_embedded" );
     return count_new_auxiliary_sequence( nir_misc::matTovec( inverseTransform ) );
}

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
