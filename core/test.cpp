#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "test.h"
#include "misc.h"
#include "dft.h"
#include "embedding.h"
#include "log.h"
#include "cv_dft.h"
#include "cv_emb.h"
using namespace cv;

int static find_max_img_elem( std::vector<std::vector<float> > img )
{
     float ret = 0;
     float buf = 0;
     for( size_t i = 0; i < img.size(); ++i )
     {
          buf = *std::max_element( img[ i ].begin(), img[ i ].end() );
          ret = buf > ret ? buf : ret;
     }
     return ret;
}
namespace nir_test
{
// void test_vec1()
// {
//      std::vector<std::vector<float> > img_v( 8 );
//      img_v[ 0 ].push_back( 131 );
//      img_v[ 0 ].push_back( 131 );
//      img_v[ 0 ].push_back( 129 );
//      img_v[ 0 ].push_back( 128 );
//      img_v[ 0 ].push_back( 132 );
//      img_v[ 0 ].push_back( 126 );
//      img_v[ 0 ].push_back( 129 );
//      img_v[ 0 ].push_back( 129 );

//      img_v[ 1 ].push_back( 135 );
//      img_v[ 1 ].push_back( 139 );
//      img_v[ 1 ].push_back( 136 );
//      img_v[ 1 ].push_back( 136 );
//      img_v[ 1 ].push_back( 141 );
//      img_v[ 1 ].push_back( 129 );
//      img_v[ 1 ].push_back( 130 );
//      img_v[ 1 ].push_back( 133 );

//      img_v[ 2 ].push_back( 131 );
//      img_v[ 2 ].push_back( 133 );
//      img_v[ 2 ].push_back( 131 );
//      img_v[ 2 ].push_back( 128 );
//      img_v[ 2 ].push_back( 130 );
//      img_v[ 2 ].push_back( 134 );
//      img_v[ 2 ].push_back( 140 );
//      img_v[ 2 ].push_back( 134 );
//      img_v[ 3 ].push_back( 125 );
//      img_v[ 3 ].push_back( 126 );
//      img_v[ 3 ].push_back( 125 );
//      img_v[ 3 ].push_back( 120 );
//      img_v[ 3 ].push_back( 123 );
//      img_v[ 3 ].push_back( 124 );
//      img_v[ 3 ].push_back( 131 );
//      img_v[ 3 ].push_back( 124 );
//      img_v[ 4 ].push_back( 131 );
//      img_v[ 4 ].push_back( 129 );
//      img_v[ 4 ].push_back( 131 );
//      img_v[ 4 ].push_back( 121 );
//      img_v[ 4 ].push_back( 125 );
//      img_v[ 4 ].push_back( 124 );
//      img_v[ 4 ].push_back( 121 );
//      img_v[ 4 ].push_back( 122 );
//      img_v[ 5 ].push_back( 130 );
//      img_v[ 5 ].push_back( 128 );
//      img_v[ 5 ].push_back( 125 );
//      img_v[ 5 ].push_back( 119 );
//      img_v[ 5 ].push_back( 119 );
//      img_v[ 5 ].push_back( 120 );
//      img_v[ 5 ].push_back( 119 );
//      img_v[ 5 ].push_back( 122 );
//      img_v[ 6 ].push_back( 117 );
//      img_v[ 6 ].push_back( 125 );
//      img_v[ 6 ].push_back( 124 );
//      img_v[ 6 ].push_back( 122 );
//      img_v[ 6 ].push_back( 121 );
//      img_v[ 6 ].push_back( 116 );
//      img_v[ 6 ].push_back( 116 );
//      img_v[ 6 ].push_back( 114 );
//      img_v[ 7 ].push_back( 105 );
//      img_v[ 7 ].push_back( 113 );
//      img_v[ 7 ].push_back( 121 );
//      img_v[ 7 ].push_back( 120 );
//      img_v[ 7 ].push_back( 126 );
//      img_v[ 7 ].push_back( 120 );
//      img_v[ 7 ].push_back( 117 );
//      img_v[ 7 ].push_back( 111 );

//      nir_dft img = nir_dft( img_v );
//      std::cout << "IMG\n";
//      nir_misc::print_vec( img.get_img() );
//      std::cout << "im\n";
//      nir_misc::print_vec( img.im() );
//      std::cout << "re\n";
//      nir_misc::print_vec( img.re() );
//      std::cout << "amp\n";
//      nir_misc::print_vec( img.calc_amp( img.im(), img.re() ) );
//      std::cout << "phase\n";
//      nir_misc::print_vec( img.calc_phase( img.im(), img.re() ) );
//      std::cout << "dft\n";
//      nir_misc::print_vec( img.do_dft() );

//      std::cout << "was vec\n";
//      // nir_embedding emb = nir_embedding(img_v, img.calc_phase(img.im(), img.re()), img.calc_amp(img.im(), img.re()), img.im(), img.re());

//      // std::cout << emb.average_count_ << std::endl;
// }
void test()
{
     Mat small_mat = Mat( 8, 8, 0 );
     small_mat.at<uchar>( 0, 0 ) = 131;
     small_mat.at<uchar>( 0, 1 ) = 131;
     small_mat.at<uchar>( 0, 2 ) = 129;
     small_mat.at<uchar>( 0, 3 ) = 128;
     small_mat.at<uchar>( 0, 4 ) = 132;
     small_mat.at<uchar>( 0, 5 ) = 126;
     small_mat.at<uchar>( 0, 6 ) = 129;
     small_mat.at<uchar>( 0, 7 ) = 129;

     small_mat.at<uchar>( 1, 0 ) = 135;
     small_mat.at<uchar>( 1, 1 ) = 139;
     small_mat.at<uchar>( 1, 2 ) = 136;
     small_mat.at<uchar>( 1, 3 ) = 136;
     small_mat.at<uchar>( 1, 4 ) = 141;
     small_mat.at<uchar>( 1, 5 ) = 129;
     small_mat.at<uchar>( 1, 6 ) = 130;
     small_mat.at<uchar>( 1, 7 ) = 133;

     small_mat.at<uchar>( 2, 0 ) = 131;
     small_mat.at<uchar>( 2, 1 ) = 133;
     small_mat.at<uchar>( 2, 2 ) = 131;
     small_mat.at<uchar>( 2, 3 ) = 128;
     small_mat.at<uchar>( 2, 4 ) = 130;
     small_mat.at<uchar>( 2, 5 ) = 134;
     small_mat.at<uchar>( 2, 6 ) = 140;
     small_mat.at<uchar>( 2, 7 ) = 134;

     small_mat.at<uchar>( 3, 0 ) = 125;
     small_mat.at<uchar>( 3, 1 ) = 126;
     small_mat.at<uchar>( 3, 2 ) = 125;
     small_mat.at<uchar>( 3, 3 ) = 120;
     small_mat.at<uchar>( 3, 4 ) = 123;
     small_mat.at<uchar>( 3, 5 ) = 124;
     small_mat.at<uchar>( 3, 6 ) = 131;
     small_mat.at<uchar>( 3, 7 ) = 124;

     small_mat.at<uchar>( 4, 0 ) = 131;
     small_mat.at<uchar>( 4, 1 ) = 129;
     small_mat.at<uchar>( 4, 2 ) = 131;
     small_mat.at<uchar>( 4, 3 ) = 121;
     small_mat.at<uchar>( 4, 4 ) = 125;
     small_mat.at<uchar>( 4, 5 ) = 124;
     small_mat.at<uchar>( 4, 6 ) = 121;
     small_mat.at<uchar>( 4, 7 ) = 122;

     small_mat.at<uchar>( 5, 0 ) = 130;
     small_mat.at<uchar>( 5, 1 ) = 128;
     small_mat.at<uchar>( 5, 2 ) = 125;
     small_mat.at<uchar>( 5, 3 ) = 119;
     small_mat.at<uchar>( 5, 4 ) = 119;
     small_mat.at<uchar>( 5, 5 ) = 120;
     small_mat.at<uchar>( 5, 6 ) = 119;
     small_mat.at<uchar>( 5, 7 ) = 122;

     small_mat.at<uchar>( 6, 0 ) = 117;
     small_mat.at<uchar>( 6, 1 ) = 125;
     small_mat.at<uchar>( 6, 2 ) = 124;
     small_mat.at<uchar>( 6, 3 ) = 122;
     small_mat.at<uchar>( 6, 4 ) = 121;
     small_mat.at<uchar>( 6, 5 ) = 116;
     small_mat.at<uchar>( 6, 6 ) = 116;
     small_mat.at<uchar>( 6, 7 ) = 114;

     small_mat.at<uchar>( 7, 0 ) = 105;
     small_mat.at<uchar>( 7, 1 ) = 113;
     small_mat.at<uchar>( 7, 2 ) = 121;
     small_mat.at<uchar>( 7, 3 ) = 120;
     small_mat.at<uchar>( 7, 4 ) = 126;
     small_mat.at<uchar>( 7, 5 ) = 120;
     small_mat.at<uchar>( 7, 6 ) = 117;
     small_mat.at<uchar>( 7, 7 ) = 111;
 std::vector<float> message = {0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0};
     //0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0
     const int Acrit = 10; // todo fixme
     const float area_width = nir_misc::count_area_width( message.size(), 1 );
// nir_cv_emb idft = nir_cv_emb( small_mat, area_width, message);
// cv::Mat out;
// idft.get_img( out);
// nir_misc::print_Mat( out);
// idft.get_phase( out);
// nir_misc::print_Mat( out);
// return;}
    //  const int k = 1; // todo сделать подсчет блоков в пикче
    //  std::vector<float> message = {0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0};
    //  //0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0
    //  const int Acrit = 10; // todo fixme
    //  const float area_width = nir_misc::count_area_width( message.size(), k );

     // nir_cv_dft dft = nir_cv_dft(small_mat);
     // cv::Mat newdft;
     // nir_cv_dft::do_dft(dft.phase, dft.amp, dft.re, newdft);

     // for (int i = 0; i < 16; i++)
     // {
     //     for (int j = 0; j < 16; j++)
     //     {
     //         std::cout.width(15);
     //         std::cout << std::to_string(dft.re.at<float>(i, j));
     //     }
     //     std::cout << std::endl;
     // }
     // std::cout << std::endl
     //           << std::endl
     //           << std::endl;
     // for (int i = 0; i < 16; i++)
     // {
     //     for (int j = 0; j < 16; j++)
     //     {
     //         std::cout.width(15);
     //         std::cout << std::to_string(dft.dft.at<float>(i, j));
     //     }
     //     std::cout << std::endl;
     // }

     // return;
     nir_cv_emb fImage = nir_cv_emb( small_mat, area_width, message , 3);

     // std::cout << fImage.get_average_count() << std::endl;
     nir_misc::printv( fImage.auxiliary_sequence );
     nir_misc::printv( fImage.best_embedding );
}

// void test_vec()
// {
//      std::vector<std::vector<float> > img_v( 8 );
//      img_v[ 0 ].push_back( 131 );
//      img_v[ 0 ].push_back( 131 );
//      img_v[ 0 ].push_back( 129 );
//      img_v[ 0 ].push_back( 128 );
//      img_v[ 0 ].push_back( 132 );
//      img_v[ 0 ].push_back( 126 );
//      img_v[ 0 ].push_back( 129 );
//      img_v[ 0 ].push_back( 129 );

//      img_v[ 1 ].push_back( 135 );
//      img_v[ 1 ].push_back( 139 );
//      img_v[ 1 ].push_back( 136 );
//      img_v[ 1 ].push_back( 136 );
//      img_v[ 1 ].push_back( 141 );
//      img_v[ 1 ].push_back( 129 );
//      img_v[ 1 ].push_back( 130 );
//      img_v[ 1 ].push_back( 133 );

//      img_v[ 2 ].push_back( 131 );
//      img_v[ 2 ].push_back( 133 );
//      img_v[ 2 ].push_back( 131 );
//      img_v[ 2 ].push_back( 128 );
//      img_v[ 2 ].push_back( 130 );
//      img_v[ 2 ].push_back( 134 );
//      img_v[ 2 ].push_back( 140 );
//      img_v[ 2 ].push_back( 134 );
//      img_v[ 3 ].push_back( 125 );
//      img_v[ 3 ].push_back( 126 );
//      img_v[ 3 ].push_back( 125 );
//      img_v[ 3 ].push_back( 120 );
//      img_v[ 3 ].push_back( 123 );
//      img_v[ 3 ].push_back( 124 );
//      img_v[ 3 ].push_back( 131 );
//      img_v[ 3 ].push_back( 124 );
//      img_v[ 4 ].push_back( 131 );
//      img_v[ 4 ].push_back( 129 );
//      img_v[ 4 ].push_back( 131 );
//      img_v[ 4 ].push_back( 121 );
//      img_v[ 4 ].push_back( 125 );
//      img_v[ 4 ].push_back( 124 );
//      img_v[ 4 ].push_back( 121 );
//      img_v[ 4 ].push_back( 122 );
//      img_v[ 5 ].push_back( 130 );
//      img_v[ 5 ].push_back( 128 );
//      img_v[ 5 ].push_back( 125 );
//      img_v[ 5 ].push_back( 119 );
//      img_v[ 5 ].push_back( 119 );
//      img_v[ 5 ].push_back( 120 );
//      img_v[ 5 ].push_back( 119 );
//      img_v[ 5 ].push_back( 122 );
//      img_v[ 6 ].push_back( 117 );
//      img_v[ 6 ].push_back( 125 );
//      img_v[ 6 ].push_back( 124 );
//      img_v[ 6 ].push_back( 122 );
//      img_v[ 6 ].push_back( 121 );
//      img_v[ 6 ].push_back( 116 );
//      img_v[ 6 ].push_back( 116 );
//      img_v[ 6 ].push_back( 114 );
//      img_v[ 7 ].push_back( 105 );
//      img_v[ 7 ].push_back( 113 );
//      img_v[ 7 ].push_back( 121 );
//      img_v[ 7 ].push_back( 120 );
//      img_v[ 7 ].push_back( 126 );
//      img_v[ 7 ].push_back( 120 );
//      img_v[ 7 ].push_back( 117 );
//      img_v[ 7 ].push_back( 111 );

//      // nir_dft img = nir_dft(img_v);
//      // nir_misc::print_vec(img.do_dft(img.calc_phase(img.im(), img.re()), img.calc_amp(img.im(), img.re()), img.im()));
//      //  cv::Mat inverseTransform;
//      //  cv::Mat ft ;
//      //  nir_misc::vecToMat(img.do_dft( img.calc_phase( img.im(), img.re() ), img.calc_amp( img.im(), img.re() ), img.im()) , img.re(), ft);
//      //     cv::dft(ft, inverseTransform, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
//      //     std::cout << "printMAT(fourierTransform);\n";
//      //     nir_misc::print_Mat(ft);
//      //     nir_misc::print_Mat(inverseTransform);

//      //     return;
//      // }
//      //} // namespace nir_test
//      nir_dft img = nir_dft( img_v );
//      const int k = 1; // todo сделать подсчет блоков в пикче
//      std::vector<float> message = {0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0};
//      //0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0
//      const int Acrit = 10; // todo fixme
//      const float area_width = nir_misc::count_area_width( message.size(), k );
//      if( -1 == area_width )
//      {
//           nir_log::error( "area width < 0" );
//           return;
//      }

//      nir_embedding emb = nir_embedding( img_v, img.calc_phase( img.im(), img.re() ), img.calc_amp( img.im(), img.re() ), img.im(), img.re() );

//      if( !emb.may_we_embedding( Acrit ) )
//      {
//           nir_log::error( "can't be embedded in this block" );
//           return;
//      }
//      std::cout << "Амп " << std::endl;
//      nir_misc::print_vec( img.calc_amp( img.im(), img.re() ) );

//      std::vector<float> test = emb.count_auxiliary_sequence( area_width );
//      std::cout << "вектор 1 -1 0 исходный" << std::endl;
//      nir_misc::printv( test );

//      //  std::vector<float> x_vec = emb.find_best_embedding( test, message );
//      //  std::cout << "вектор 1 -1 0 сообщение" << std::endl;
//      //  nir_misc::printv( x_vec );
//      //       for( int i = 0 ; i < 8 ; ++i)
//      //  {
//      //            for( int j = 0 ; j < 8 ; ++j)
//      //  {
//      //       int s;
//      //       std::cin >>s;
//      emb.do_test_embedded( test, {-1, 0, 1, 1, -1, 1, 1, 1, 0, -1, 0, -1, -1, -1, 0, 0, 1, 1, 1, -1, 0, 1} );
//      // emb.do_test_embedded( test, {-1, 0, 1, 0, -1, 0, 0, 0, 0, -1, 1, -1, -1, -1, 0, 1, 0, 1, 1, -1, 0, 1} );
//      // emb.do_test_embedded( test, x_vec );
//      //  }
//      //  }
// }

void test_idft()
{
     Mat small_mat = Mat( 8, 8, 0 );
     small_mat.at<uchar>( 0, 0 ) = 129;
     small_mat.at<uchar>( 0, 1 ) = 128;
     small_mat.at<uchar>( 0, 2 ) = 131;
     small_mat.at<uchar>( 0, 3 ) = 129;
     small_mat.at<uchar>( 0, 4 ) = 133;
     small_mat.at<uchar>( 0, 5 ) = 126;
     small_mat.at<uchar>( 0, 6 ) = 130;
     small_mat.at<uchar>( 0, 7 ) = 130;

     small_mat.at<uchar>( 1, 0 ) = 133;
     small_mat.at<uchar>( 1, 1 ) = 138;
     small_mat.at<uchar>( 1, 2 ) = 140;
     small_mat.at<uchar>( 1, 3 ) = 134;
     small_mat.at<uchar>( 1, 4 ) = 136;
     small_mat.at<uchar>( 1, 5 ) = 131;
     small_mat.at<uchar>( 1, 6 ) = 134;
     small_mat.at<uchar>( 1, 7 ) = 133;

     small_mat.at<uchar>( 2, 0 ) = 136;
     small_mat.at<uchar>( 2, 1 ) = 136;
     small_mat.at<uchar>( 2, 2 ) = 128;
     small_mat.at<uchar>( 2, 3 ) = 127;
     small_mat.at<uchar>( 2, 4 ) = 129;
     small_mat.at<uchar>( 2, 5 ) = 135;
     small_mat.at<uchar>( 2, 6 ) = 139;
     small_mat.at<uchar>( 2, 7 ) = 131;

     small_mat.at<uchar>( 3, 0 ) = 124;
     small_mat.at<uchar>( 3, 1 ) = 126;
     small_mat.at<uchar>( 3, 2 ) = 125;
     small_mat.at<uchar>( 3, 3 ) = 122;
     small_mat.at<uchar>( 3, 4 ) = 124;
     small_mat.at<uchar>( 3, 5 ) = 123;
     small_mat.at<uchar>( 3, 6 ) = 130;
     small_mat.at<uchar>( 3, 7 ) = 123;

     small_mat.at<uchar>( 4, 0 ) = 128;
     small_mat.at<uchar>( 4, 1 ) = 126;
     small_mat.at<uchar>( 4, 2 ) = 131;
     small_mat.at<uchar>( 4, 3 ) = 126;
     small_mat.at<uchar>( 4, 4 ) = 125;
     small_mat.at<uchar>( 4, 5 ) = 121;
     small_mat.at<uchar>( 4, 6 ) = 123;
     small_mat.at<uchar>( 4, 7 ) = 125;

     small_mat.at<uchar>( 5, 0 ) = 129;
     small_mat.at<uchar>( 5, 1 ) = 128;
     small_mat.at<uchar>( 5, 2 ) = 123;
     small_mat.at<uchar>( 5, 3 ) = 118;
     small_mat.at<uchar>( 5, 4 ) = 119;
     small_mat.at<uchar>( 5, 5 ) = 119;
     small_mat.at<uchar>( 5, 6 ) = 121;
     small_mat.at<uchar>( 5, 7 ) = 124;

     small_mat.at<uchar>( 6, 0 ) = 118;
     small_mat.at<uchar>( 6, 1 ) = 129;
     small_mat.at<uchar>( 6, 2 ) = 125;
     small_mat.at<uchar>( 6, 3 ) = 118;
     small_mat.at<uchar>( 6, 4 ) = 122;
     small_mat.at<uchar>( 6, 5 ) = 117;
     small_mat.at<uchar>( 6, 6 ) = 113;
     small_mat.at<uchar>( 6, 7 ) = 113;

     small_mat.at<uchar>( 7, 0 ) = 108;
     small_mat.at<uchar>( 7, 1 ) = 113;
     small_mat.at<uchar>( 7, 2 ) = 119;
     small_mat.at<uchar>( 7, 3 ) = 121;
     small_mat.at<uchar>( 7, 4 ) = 129;
     small_mat.at<uchar>( 7, 5 ) = 120;
     small_mat.at<uchar>( 7, 6 ) = 112;
     small_mat.at<uchar>( 7, 7 ) = 110;


   std::vector<float> message = {0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0};
     //0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0
     const int Acrit = 10; // todo fixme
     const float area_width = nir_misc::count_area_width( message.size(), 1 );
nir_cv_dft idft = nir_cv_dft( small_mat );
cv::Mat out;
nir_misc::print_Mat( idft.phase);
nir_misc::print_Mat( idft.img);
}
} // namespace nir_test
