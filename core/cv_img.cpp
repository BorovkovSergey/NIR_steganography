#include "cv_img.h"

#include <utility>

#include <opencv/cv.h>
#include <opencv2/highgui.hpp>

static const int block_size = 8;

nir_cv_img::nir_cv_img( const std::string& input )
     : img( cv::imread( input, 0 ) )
{
     img.convertTo( out_img, 0 );
     out_img = cv::Mat( img.rows, img.cols, 0 );
     do_blocks();
     do_new_img();
}

void nir_cv_img::do_blocks()
{
     int it = 0;
     int a, b;
     bool flag = false;
     std::pair<int, int> cols_range = {0, 7};
     std::pair<int, int> rows_range = {0, 7};
     cv::Mat buf( 8, 8, 0 );
     for( unsigned int i = rows_range.first;; ++i )
     {
          for( unsigned int j = cols_range.first; j <= cols_range.second; ++j )
          {
               if( i >= img.rows && j == img.cols - 1 )
               {
                    vec_img.push_back( buf );
                    return;
               }
               buf.at<uchar>( i % 8, j % 8 ) = img.at<uchar>( i, j );

               if( j >= img.cols - 1 && i >= rows_range.second )
               {
                    rows_range.first += block_size;
                    rows_range.second += block_size;

                    cols_range.first = 0;
                    cols_range.second = 7;

                    vec_img.push_back( buf );
                    buf = cv::Mat( 8, 8, 0 );
                    ++it;
                    continue;
               }
          }
          if( i == rows_range.second )
          {
               cols_range.first += block_size;
               cols_range.second += block_size;
               i = rows_range.first - 1;

               vec_img.push_back( buf );
               buf = cv::Mat( 8, 8, 0 );
               ++it;
          }
     }
}

void nir_cv_img::do_new_img()
{
     int it = 0;
     int a, b;
     bool flag = false;
     std::pair<int, int> cols_range = {0, 7};
     std::pair<int, int> rows_range = {0, 7};
     for( unsigned int i = rows_range.first; it < vec_img.size(); ++i )
     {
          for( unsigned int j = cols_range.first; j <= cols_range.second; ++j )
          {
               if( i >= img.rows && j == img.cols - 1 )
               {
                    std::cout << it << std::endl;
                    return;
               }
               out_img.at<uchar>( i, j ) = vec_img[ it ].at<uchar>( i % 8, j % 8 );

               if( j >= img.cols - 1 && i >= rows_range.second )
               {
                    rows_range.first += block_size;
                    rows_range.second += block_size;

                    cols_range.first = 0;
                    cols_range.second = 7;
                    ++it;
                    break;
               }
          }
          if( i == rows_range.second )
          {
               cols_range.first += block_size;
               cols_range.second += block_size;
               i = rows_range.first - 1;
               ++it;
          }
     }
}
