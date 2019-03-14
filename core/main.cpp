#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <map>
#include "misc.h"
#include "dft.h"
#include "test.h"
#include <vector>
#include "log.h"
#include "cv_img.h"
float re_( const std::vector<std::vector<float> >& img_, const float x, const float y )
{
     float ans = 0;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ans += img_[ i ][ j ] * cos( -1 * ( i * x / 8 + j * y / 8 ) * M_PI * 2 );
          }
     }
     return ans / 64;
}
vecImg re( const std::vector<std::vector<float> >& img_ )
{
     nir_log::info( "Start re" );
     vecImg ret = img_;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ret[ i ][ j ] = re_( img_, i, j );
          }
     }
     nir_log::info( "End re" );
     return ret;
}

float ire_( const std::vector<std::vector<float> >& img_, const float x, const float y )
{
     float ans = 0;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ans += img_[ i ][ j ] * cos( ( ( i * x / ( img_.size() + 1 ) ) + ( j * y / ( img_.size() + 1 ) ) ) * M_PI * 2 );
          }
     }
     return ans;
}
vecImg ire( const std::vector<std::vector<float> >& img_ )
{
     nir_log::info( "Start re" );
     vecImg ret = img_;
     for( int i = 0; i < img_.size(); i++ )
     {
          for( int j = 0; j < img_.size(); j++ )
          {
               ret[ i ][ j ] = ire_( img_, i, j );
          }
     }
     nir_log::info( "End re" );
     return ret;
}


// Message can't be const cuz will erase in prepare_substr!!!
int main()
{
     const std::string pic = "Lenna1.jpeg";
     std::string s = "1234567890";
     std::cout << nir_misc::preapare_substr(s, 5) << " " << s;
     // nir_cv_img img = nir_cv_img( pic );

     // imshow( "disp1lay", img.img );
     // cvWaitKey( 0 );
     // imshow( "display", img.out_img );
     // cvWaitKey( 0 );
     //  nir_test::test();
     //  std::vector<std::vector<float> > img_v( 8 );
     //  img_v[ 0 ].push_back( 131 );
     //  img_v[ 0 ].push_back( 131 );
     //  img_v[ 0 ].push_back( 129 );
     //  img_v[ 0 ].push_back( 128 );
     //  img_v[ 0 ].push_back( 132 );
     //  img_v[ 0 ].push_back( 126 );
     //  img_v[ 0 ].push_back( 129 );
     //  img_v[ 0 ].push_back( 129 );

     //  img_v[ 1 ].push_back( 135 );
     //  img_v[ 1 ].push_back( 139 );
     //  img_v[ 1 ].push_back( 136 );
     //  img_v[ 1 ].push_back( 136 );
     //  img_v[ 1 ].push_back( 141 );
     //  img_v[ 1 ].push_back( 129 );
     //  img_v[ 1 ].push_back( 130 );
     //  img_v[ 1 ].push_back( 133 );

     //  img_v[ 2 ].push_back( 131 );
     //  img_v[ 2 ].push_back( 133 );
     //  img_v[ 2 ].push_back( 131 );
     //  img_v[ 2 ].push_back( 128 );
     //  img_v[ 2 ].push_back( 130 );
     //  img_v[ 2 ].push_back( 134 );
     //  img_v[ 2 ].push_back( 140 );
     //  img_v[ 2 ].push_back( 134 );
     //  img_v[ 3 ].push_back( 125 );
     //  img_v[ 3 ].push_back( 126 );
     //  img_v[ 3 ].push_back( 125 );
     //  img_v[ 3 ].push_back( 120 );
     //  img_v[ 3 ].push_back( 123 );
     //  img_v[ 3 ].push_back( 124 );
     //  img_v[ 3 ].push_back( 131 );
     //  img_v[ 3 ].push_back( 124 );
     //  img_v[ 4 ].push_back( 131 );
     //  img_v[ 4 ].push_back( 129 );
     //  img_v[ 4 ].push_back( 131 );
     //  img_v[ 4 ].push_back( 121 );
     //  img_v[ 4 ].push_back( 125 );
     //  img_v[ 4 ].push_back( 124 );
     //  img_v[ 4 ].push_back( 121 );
     //  img_v[ 4 ].push_back( 122 );
     //  img_v[ 5 ].push_back( 130 );
     //  img_v[ 5 ].push_back( 128 );
     //  img_v[ 5 ].push_back( 125 );
     //  img_v[ 5 ].push_back( 119 );
     //  img_v[ 5 ].push_back( 119 );
     //  img_v[ 5 ].push_back( 120 );
     //  img_v[ 5 ].push_back( 119 );
     //  img_v[ 5 ].push_back( 122 );
     //  img_v[ 6 ].push_back( 117 );
     //  img_v[ 6 ].push_back( 125 );
     //  img_v[ 6 ].push_back( 124 );
     //  img_v[ 6 ].push_back( 122 );
     //  img_v[ 6 ].push_back( 121 );
     //  img_v[ 6 ].push_back( 116 );
     //  img_v[ 6 ].push_back( 116 );
     //  img_v[ 6 ].push_back( 114 );
     //  img_v[ 7 ].push_back( 105 );
     //  img_v[ 7 ].push_back( 113 );
     //  img_v[ 7 ].push_back( 121 );
     //  img_v[ 7 ].push_back( 120 );
     //  img_v[ 7 ].push_back( 126 );
     //  img_v[ 7 ].push_back( 120 );
     //  img_v[ 7 ].push_back( 117 );
     //  img_v[ 7 ].push_back( 111 );

     //  nir_misc::print_vec( re( img_v ) );
     //  nir_misc::print_vec( ire( re( img_v ) ) );

     //  nir_dft dft = nir_dft(img_v);
     //  dft.do_dft( dft.calc_phase( dft.im(), dft.re() ) , dft.calc_amp(dft.im(), dft.re() ));
     //  cv::Mat ret;
     //  cv::Mat idft;
     //  nir_misc::vecToMat( dft.do_dft( dft.calc_phase( dft.im(), dft.re() ) , dft.calc_amp(dft.im(), dft.re() )), dft.re(), ret );
     //  nir_misc::print_Mat( ret );
     //  cv::dft( ret, idft, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT );
     //  nir_misc::print_vec( img_v);
     //  nir_misc::print_Mat( idft);
     return 0;
}
