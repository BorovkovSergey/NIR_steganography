#pragma once
#include <opencv2/core/core.hpp>

class nir_img
{
public:
     // for type see opencv doc cv::Mat
     nir_img( char* img_name, int type );
     nir_img( cv::Mat& img );
     enum
     {
          BLUE = 0,
          GREEN = 1,
          RED = 2
     } RGB; // todo delete if will not using

     // @brief create discrete fourier transform
     cv::Mat DFT() const;
     
     // @brief functions for create DFT phase coefficient matrix
     cv::Mat get_DFT_phase() const;

     // @brief functions for create DFT amplitude
     cv::Mat get_DFT_amplitude() const;

     // @brief function for get img_
     cv::Mat get_img() const;

   private:
     cv::Mat img_;
};

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
