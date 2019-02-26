#include "dft.h"
#include <math.h>
#include <iostream>

nir_dft::nir_dft( vecImg& im )
     : img_( im )
{
}

nir_dft::nir_dft( cv::Mat& im )
     : img_( im.rows )
{
     for( unsigned int i = 0; i < im.rows; i++ ) // todo convvert Mat to vecImg!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 26.02.2019
     {
          for( unsigned j = 0; j < im.cols; j++ )
          {
               img_[ i ].push_back( im.at<float>( i, j ) );
          }
     }
}

float nir_dft::re_( float x, float y ) const
{
     float ans = 0;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ans += img_[ i ][ j ] * cosf32( -1 * ( i * x / img_.size() + j * y / img_.size() ) * M_PI * 2 );
          }
     }

     return ans / 64;
}

float nir_dft::im_( float x, float y ) const
{
     float ans = 0;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ans += img_[ i ][ j ] * sinf32( -1 * ( i * x / img_.size() + j * y / img_.size() ) * M_PI * 2 );
          }
     }

     return ans / 64;
}
vecImg nir_dft::calc_phase( vecImg im, vecImg re ) const
{
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
     return ret;
}

vecImg nir_dft::calc_amp( vecImg im, vecImg re ) const
{
     vecImg ret = re;
     for( int i = 0; i < re.size(); i++ )
     {
          for( int j = 0; j < re.size(); j++ )
          {
               ret[ i ][ j ] = sqrt( im[ i ][ j ] * im[ i ][ j ] + re[ i ][ j ] * re[ i ][ j ] );
          }
     }
     return ret;
}

vecImg nir_dft::im() const
{
     vecImg ret = img_;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ret[ i ][ j ] = im_( i, j );
          }
     }
     return ret;
}
vecImg nir_dft::re() const
{
     vecImg ret = img_;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ret[ i ][ j ] = re_( i, j );
          }
     }
     return ret;
}
vecImg nir_dft::complex() const
{
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
     return ret;
}
vecImg nir_dft::do_dft() const
{
     vecImg ret = complex();
     for( auto& v : ret )
     {
          v.resize( v.size() / 2 );
     }
     return ret;
}
vecImg nir_dft::do_dft( vecImg phase_, vecImg ampl_, vecImg im ) const
{
     bool minus = false;
     vecImg ret = phase_;
     for( int i = 0; i < ret.size(); i++ )
     {
          for( int j = 0; j < ret.size(); j++ )
          {
               if( ( 0 > phase_[ i ][ j ] && 0 < im[ i ][ j ] ) ||
                   ( 0 < phase_[ i ][ j ] && 0 > im[ i ][ j ] ) )
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
                         ret[ i ][ j ] = ampl_[ i ][ j ] * cos( phase_[ i ][ j ] );
                         break;

                    case 1:
                         ret[ i ][ 2 ] = ampl_[ i ][ j ] * cos( phase_[ i ][ j ] );
                         break;

                    case 2:
                         ret[ i ][ 4 ] = ampl_[ i ][ j ] * cos( phase_[ i ][ j ] );
                         break;

                    case 3:
                         ret[ i ][ 6 ] = ampl_[ i ][ j ] * cos( phase_[ i ][ j ] );
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
     return ret;
}
