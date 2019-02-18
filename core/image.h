#pragma once
#include <opencv2/core/core.hpp>

class nir_img
{
     nir_img( char* img_name );
public:
     enum
     {
          BLUE = 0,
          GREEN = 1,
          RED = 2
     } RGB; // todo delete if will not using

     void DFT( cv::Mat& img );
private:
     cv::Mat img_;
}

/*
     пробежка по пикселям и построение решеточки
     for( int i = 0; i < im.rows; i++ )
     {
          for( int j = 0; j < im.cols; j++ )
          {
               if( ( i % 20 == 10 && j % 2 == 1 ) ||
                   ( j % 50 == 25 && i % 2 == 1 ) )
               {
                    // may insert this values into int!!!
                    im.at<Vec3b>( i, j )[ BLUE ] = 255;
                    im.at<Vec3b>( i, j )[ GREEN ] = 255;
                    im.at<Vec3b>( i, j )[ RED ] = 255;
               }
          }
     }
*/

/*
преобразование фурье ( не тестил )
std::vector<std::complex<float> > fourierCV; // output
cv::dft( std::vector<float>( centroidDistance, centroidDistance + this->N ), fourierCV, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT );
*/
