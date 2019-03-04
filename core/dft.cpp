#include "dft.h"
#include "log.h"
#include "misc.h"

#include <math.h>
#include <iostream>
#include <string>

static std::vector<std::vector<float> > rounding( std::vector<std::vector<float> > img )
{
     nir_log::info( "Start nir_dft rounding" );

     int buf;
     for( size_t i = 0; i < img.size(); ++i )
     {
          for( size_t j = 0; j < img.size(); ++j )
          {
               buf = img[ i ][ j ];
               if( img[ i ][ j ] - (float)buf > 0.5 )
               {
                    ++buf;
               }
               img[ i ][ j ] = buf;
          }
     }
     nir_log::info( "End nir_dft rounding" );

     return img;
}
nir_dft::nir_dft( const vecImg& im )
     : img_( rounding( im ) )
// : img_( rounding( im ) )
{
     nir_log::info( "Start nir_dft constructor vec" );

     nir_log::info( "End nir_dft constructor vec" );
}

nir_dft::nir_dft( const cv::Mat& im )
     : img_( im.rows )
{
     nir_log::info( "Start nir_dft constructor Mat" );

     for( unsigned int i = 0; i < im.rows; i++ )
     {
          for( unsigned j = 0; j < im.cols; j++ )
          {
               img_[ i ].push_back( im.at<float>( i, j ) );
          }
     }
     nir_log::info( "End nir_dft constructor Mat" );
}

float nir_dft::re_( const float x, const float y ) const
{
     float ans = 0;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ans += img_[ i ][ j ] * cos( -1 * ( i * x / img_.size() + j * y / img_.size() ) * M_PI * 2 );
          }
     }
     return ans / 64;
}

float nir_dft::im_( const float x, const float y ) const
{
     float ans = 0;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ans += img_[ i ][ j ] * sin( -1 * ( i * x / img_.size() + j * y / img_.size() ) * M_PI * 2 );
          }
     }
     return ans / 64;
}
vecImg nir_dft::calc_phase( const vecImg& im, const vecImg& re ) const
{
     nir_log::info( "Start calc_phase" );
     vecImg ret = re;
     for( int j = 0; j < re.size(); j++ )
     {
          for( int i = 0; i < re.size(); i++ )
          {
               if( ( i > 4 || j > 4 ) && ( i < 1 || i > 3 ) )
               {
                    ret[ i ][ j ] = -atan2f32( im[ ( 8 - i ) % 8 ][ ( 8 - j ) % 8 ], re[ ( 8 - i ) % 8 ][ ( 8 - j ) % 8 ] );
                    continue;
               }
               ret[ i ][ j ] = atan2f32( im[ i ][ j ], re[ i ][ j ] );
          }
     }
     nir_log::info( "End calc_phase" );
     return ret;
}

vecImg nir_dft::calc_amp( const vecImg& im, const vecImg& re ) const
{
     nir_log::info( "Start calc_amp" );
     vecImg ret = re;
     for( int i = 0; i < re.size(); i++ )
     {
          for( int j = 0; j < re.size(); j++ )
          {
               ret[ i ][ j ] = sqrt( im[ i ][ j ] * im[ i ][ j ] + re[ i ][ j ] * re[ i ][ j ] );
          }
     }
     nir_log::info( "End calc_amp" );
     return ret;
}

vecImg nir_dft::im() const
{
     nir_log::info( "Start im" );
     vecImg ret = img_;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ret[ i ][ j ] = im_( i, j );
          }
     }
     nir_log::info( "End im" );
     return ret;
}
vecImg nir_dft::re() const
{
     nir_log::info( "Start re" );
     vecImg ret = img_;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ret[ i ][ j ] = re_( i, j );
          }
     }
     nir_log::info( "End re" );
     return ret;
}
vecImg nir_dft::complex() const
{
     nir_log::info( "Start complex" );
     vecImg ret( img_ );
     int pos = 0;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_[ i ].size(); j++ )
          {
               if( ret.size() == ret[ i ].size() )
               {
                    ret[ i ].resize( ret[ i ].size() * 2 );
               }
               ret[ i ][ pos ] = re_( i, j );
               ret[ i ][ ++pos ] = im_( i, j );
               ++pos;
          }
          pos = 0;
     }
     nir_log::info( "End complex" );
     return ret;
}
vecImg nir_dft::do_dft() const
{
     nir_log::info( "Start do_dft" );
     vecImg ret = complex();
     for( auto& v : ret )
     {
          v.resize( v.size() / 2 );
     }
     nir_log::info( "End do_dft" );
     return ret;
}
vecImg nir_dft::do_dft( const vecImg& phase, const vecImg& ampl, const vecImg& im )
{
     nir_log::info( "Start do_dft with params" );
     bool minus = false;
     vecImg ret = phase;
     for( int i = 0; i < ret.size(); i++ )
     {
          for( int j = 0; j < ret.size(); j++ )
          {
               if( ( 0 > phase[ i ][ j ] && 0 < im[ i ][ j ] ) ||
                   ( 0 < phase[ i ][ j ] && 0 > im[ i ][ j ] ) )
               {
                    minus = true;
               }
               else
               {
                    minus = false;
               }
               switch( j )
               {
                    case 0:
                         ret[ i ][ j ] = ampl[ i ][ j ] * cos( phase[ i ][ j ] );
                         break;

                    case 1:
                         ret[ i ][ 2 ] = ampl[ i ][ j ] * cos( phase[ i ][ j ] );
                         break;

                    case 2:
                         ret[ i ][ 4 ] = ampl[ i ][ j ] * cos( phase[ i ][ j ] );
                         break;

                    case 3:
                         ret[ i ][ 6 ] = ampl[ i ][ j ] * cos( phase[ i ][ j ] );
                         break;
               }
               switch( j )
               {
                    case 0:
                         ret[ i ][ 1 ] = im[ i ][ j ];
                         break;

                    case 1:
                         ret[ i ][ 3 ] = im[ i ][ j ];
                         break;

                    case 2:
                         ret[ i ][ 5 ] = im[ i ][ j ];
                         break;

                    case 3:
                         ret[ i ][ 7 ] = im[ i ][ j ];
                         break;
               }
               if( minus )
               {
                    ret[ i ][ j ] = -ret[ i ][ j ];
               }
          }
     }
     nir_log::info( "End do_dft with params" );

     return ret;
}
