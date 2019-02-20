#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <map>
#include "image.h"

using namespace cv;

void printMAT( const Mat& padded )
{
     for( int i = 0; i < padded.rows; i++ )
     {
          for( int j = 0; j < padded.cols; j++ )
          {
               std::cout << padded.at<float>( i, j ) << "\t\t"; //
          }
          std::cout << "\n";
     }
     std::cout << "\n";
}
void test()
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

     cv::Mat fImage;
     small_mat.convertTo( fImage, CV_32F );

     nir_img img = nir_img( fImage );

     printMAT( img.get_DFT_phase() );
}

int main()
{
     test();
     return 0;
}
