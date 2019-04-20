#include "cv_dft.h"
#include "misc.h"
#include "log.h"
#include <iostream>
void rounding( cv::Mat& img )
{
     nir_log::info( "Start nir_dft rounding" );
     int buf;
     for( size_t i = 0; i < img.cols; ++i )
     {
          for( size_t j = 0; j < img.rows; ++j )
          {
               buf = img.at<float>( i, j );
               if( img.at<float>( i, j ) - (float)buf > 0.5 )
               {
                    ++buf;
               }
               img.at<float>( i, j ) = buf;
          }
     }
     nir_log::info( "End nir_dft rounding" );
}

nir_cv_dft::nir_cv_dft( const cv::Mat& input )
     : img()
{
     input.convertTo( img, CV_32F );
     // input.convertTo( re, CV_32F );
     // input.convertTo( im, CV_32F );
     // input.convertTo( phase, CV_32F );
     // input.convertTo( amp, CV_32F );
     // input.convertTo( dft, CV_32F );
     // img = cv::Mat( 8,8, CV_32F);
     re = cv::Mat( 8,8, CV_32F);
     im = cv::Mat( 8,8, CV_32F);
     phase = cv::Mat( 8,8, CV_32F);
     amp = cv::Mat( 8,8, CV_32F);
     dft = cv::Mat( 8,8, CV_32F);
     rounding( img );
     do_dft();
     get_im_re();
     calc_phase();
     do_real();
     calc_amp();
}
float nir_cv_dft::re_( const float x, const float y )
{
     float ans = 0;
     for( int i = 0; i < img.cols; i++ )
     {
          for( int j = 0; j < img.rows; j++ )
          {
               ans += img.at<float>( i, j ) * cos( -1 * ( i * x / 8 + j * y / 8 ) * M_PI * 2 );
          }
     }
     return ans / 64;
}
float nir_cv_dft::im_( const float x, const float y )
{
     float ans = 0;
     for( int i = 0; i < img.cols; i++ )
     {
          for( int j = 0; j < img.rows; j++ )
          {
               ans += img.at<float>( i, j ) * sin( -1 * ( i * x / 8 + j * y / 8 ) * M_PI * 2 );
          }
     }
     return ans / 64;
}
void nir_cv_dft::get_im_re()
{
     cv::Mat padded; //expand input image to optimal size
     int m = cv::getOptimalDFTSize( img.rows );
     int n = cv::getOptimalDFTSize( img.cols ); // on the border add zero values
     copyMakeBorder( img, padded, 0, m - img.rows, 0, n - img.cols, cv::BORDER_CONSTANT, cv::Scalar::all( 0 ) );

     cv::Mat planes[] = {cv::Mat_<float>( padded ), cv::Mat::zeros( padded.size(), CV_32F )};
     cv::Mat complexI;
     merge( planes, 2, complexI ); // Add to the expanded another plane with zeros

     cv::dft( complexI, complexI ); // this way the result may fit in the source matrix

     split( complexI, planes ); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
     for( unsigned int i = 0; i < re.cols; ++i )
     {
          for( unsigned int j = 0; j < re.cols; ++j )
          {
               planes[ 0 ].at<float>( i, j ) /= 64;
               planes[ 1 ].at<float>( i, j ) /= 64;
          }
     }
     re = planes[ 0 ];
     im = planes[ 1 ];
     // for( int i = 0; i < img.cols; i++ )
     // {
     //      for( int j = 0; j < img.rows; j++ )
     //      {
     //           im.at<float>( i, j ) = im_( i, j );
     //           re.at<float>( i, j ) = re_( i, j );
     //      }
     // }
}

void nir_cv_dft::calc_phase()
{
     nir_log::info( "Start calc_phase" );
     for( int j = 0; j < re.cols; j++ )
     {
          for( int i = 0; i < re.rows; i++ )
          {
               if( ( i >= 4 || j >= 4 ) && ( i < 1 || i > 3 ) )
               {
                    phase.at<float>( i, j ) = -atan2f32( im.at<float>( ( 8 - i ) % 8, ( 8 - j ) % 8 ), re.at<float>( ( 8 - i ) % 8, ( 8 - j ) % 8 ) );
                    continue;
               }
               phase.at<float>( i, j ) = atan2f32( im.at<float>( i, j ), re.at<float>( i, j ) );
          }
     }
     nir_log::info( "End calc_phase" );
}

void nir_cv_dft::calc_amp()
{
     nir_log::info( "Start calc_phase" );

//   cv::Mat padded; //expand input image to optimal size
//      int m = cv::getOptimalDFTSize( img.rows );
//      int n = cv::getOptimalDFTSize( img.cols ); // on the border add zero values
//      copyMakeBorder( img, padded, 0, m - img.rows, 0, n - img.cols, cv::BORDER_CONSTANT, cv::Scalar::all( 0 ) );

//      cv::Mat planes[] = {cv::Mat_<float>( padded ), cv::Mat::zeros( padded.size(), CV_32F )};
//      cv::Mat complexI;
//      merge( planes, 2, complexI ); // Add to the expanded another plane with zeros

//      cv::dft( complexI, complexI ); // this way the result may fit in the source matrix

//      split( complexI, planes ); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
//      for( unsigned int i = 0; i < re.cols; ++i )
//      {
//           for( unsigned int j = 0; j < re.cols; ++j )
//           {
//                planes[ 0 ].at<float>( i, j ) /= 64;
//                planes[ 1 ].at<float>( i, j ) /= 64;
//           }
//      }
     
//     magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
// amp = planes[0];

     for( int j = 0; j < re.cols; j++ )
     {
          for( int i = 0; i < re.rows; i++ )
          {
               amp.at<float>( i, j ) = sqrt( im.at<float>( i, j ) * im.at<float>( i, j ) + re.at<float>( i, j ) * re.at<float>( i, j ) );
               // real заменить на re чтобы было как в опенсиви
          }
     }
     nir_log::info( "End calc_phase" );
}

void nir_cv_dft::do_dft()
{
     cv::dft( img, dft, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT );
}
void nir_cv_dft::do_real()
{
     cv::dft( img, real, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT );
}

void nir_cv_dft::do_dft( const cv::Mat& new_phase, const cv::Mat& amplitude, cv::Mat& out )
{
     nir_log::info( "Start do_dft with params" );
     bool minus = false;
     cv::Mat ret = cv::Mat( 8, 8, 13 );
     out = cv::Mat( 8, 8, 13 );
     for( int i = 0; i < ret.cols; i++ )
     {
          for( int j = 0; j < ret.rows; j++ )
          {
               switch( j )
               {
                    case 0:
                         out.at<float>( i, j ) = amplitude.at<float>( i, j ) * cosf32( new_phase.at<float>( i, j ) );
                         break;

                    case 1:
                         out.at<float>( i, 2 ) = amplitude.at<float>( i, j ) * cosf32( new_phase.at<float>( i, j ) );
                         break;

                    case 2:
                         out.at<float>( i, 4 ) = amplitude.at<float>( i, j ) * cosf32( new_phase.at<float>( i, j ) );
                         break;

                    case 3:
                         out.at<float>( i, 6 ) = amplitude.at<float>( i, j ) * cosf32( new_phase.at<float>( i, j ) );
                         break;

                    case 4:
                         out.at<float>( i, 8 ) = amplitude.at<float>( i, j ) * cosf32( new_phase.at<float>( i, j ) );
                         break;

                    case 5:
                         out.at<float>( i, 10 ) = amplitude.at<float>( i, j ) * cosf32( new_phase.at<float>( i, j ) );
                         break;

                    case 6:
                         out.at<float>( i, 12 ) = amplitude.at<float>( i, j ) * cosf32( new_phase.at<float>( i, j ) );
                         break;

                    case 7:
                         out.at<float>( i, 14 ) = amplitude.at<float>( i, j ) * cosf32( new_phase.at<float>( i, j ) );
                         break;
               }
               switch( j )
               {
                    case 0:
                         out.at<float>( i, 1 ) = amplitude.at<float>( i, j ) * sinf32( new_phase.at<float>( i, j ) );
                         break;

                    case 1:
                         out.at<float>( i, 3 ) = amplitude.at<float>( i, j ) * sinf32( new_phase.at<float>( i, j ) );
                         break;

                    case 2:
                         out.at<float>( i, 5 ) = amplitude.at<float>( i, j ) * sinf32( new_phase.at<float>( i, j ) );
                         break;

                    case 3:
                         out.at<float>( i, 7 ) = amplitude.at<float>( i, j ) * sinf32( new_phase.at<float>( i, j ) );
                         break;

                    case 4:
                         out.at<float>( i, 9 ) = amplitude.at<float>( i, j ) * sinf32( new_phase.at<float>( i, j ) );
                         break;

                    case 5:
                         out.at<float>( i, 11 ) = amplitude.at<float>( i, j ) * sinf32( new_phase.at<float>( i, j ) );
                         break;

                    case 6:
                         out.at<float>( i, 13 ) = amplitude.at<float>( i, j ) * sinf32( new_phase.at<float>( i, j ) );
                         break;

                    case 7:
                         out.at<float>( i, 15 ) = amplitude.at<float>( i, j ) * sinf32( new_phase.at<float>( i, j ) );
                         break;
               }
          }
     }
     nir_log::info( "End do_dft with params" );
}
