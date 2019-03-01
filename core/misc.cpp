#include "misc.h"
#include <iostream>

static int cout_width_count = 13;
namespace nir_misc
{
void print_vec( std::vector<std::vector<float> > padded )
{
     for( int i = 0; i < padded.size(); i++ )
     {
          for( int j = 0; j < padded[ i ].size(); j++ )
          {
               std::cout.width( cout_width_count );
               std::cout << padded[ i ][ j ];
          }
          std::cout << "\n";
     }
     std::cout << "\n";
}

void print_Mat( cv::Mat padded )
{

     for( int i = 0; i < padded.rows; i++ )
     {
          for( int j = 0; j < padded.cols; j++ )
          {
               std::cout.width( cout_width_count );
               std::cout << padded.at<float>( i, j );
          }
          std::cout << std::endl;
     }
     std::cout << "\n";
}

void vecToMat( const std::vector<std::vector<float> >& im, const std::vector<std::vector<float> >& re, cv::Mat& ret )
{
     ret = cv::Mat( im.size(), im[ 0 ].size(), 13 );
     for( int i = 0; i < im.size(); i++ )
     {
          for( int j = 0; j < im[ i ].size(); j++ )
          {
               ret.at<float>( i, j ) = im[ i ][ j ];
          }
     }
}

float count_area_width( float message_length, float blocks_count )
{
     float ret = ( M_PI * message_length ) / ( 44 * blocks_count );
     std::cout << "Area width( epsilon ) = " << ret << std::endl;
     return ret > M_PI / 2 ? -1 : ret;
}
} // namespace nir_misc
