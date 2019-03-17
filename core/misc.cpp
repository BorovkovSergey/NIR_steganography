#include "misc.h"
#include "log.h"

#include <bitset>
#include <string>
#include <iostream>
static int cout_width_count = 15;
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
static float calculate_rmse( std::vector<std::vector<float> >& prev_img, std::vector<std::vector<float> >& modified_img )
{
     nir_log::info( "Start calculate_rmse" );
     float ret = 0;
     if( prev_img.size() != modified_img.size() )
     {
          nir_log::warning( "prev_img.size() != modified_img.size()" );
     }
     for( size_t i = 0; i < prev_img.size() && i < modified_img.size(); ++i )
     {
          for( size_t j = 0; j < prev_img.size() && j < modified_img.size(); ++j )
          {
               ret += std::pow( prev_img[ i ][ j ] + prev_img[ i ][ j ], 2 );
          }
     }
     nir_log::info( "End calculate_rmse" );
     return sqrt( ret / ( prev_img.size() * prev_img[ 0 ].size() ) );
}

namespace nir_misc
{

template <class T>
void printv( const std::vector<T>& vec )
{
     for( auto it : vec )
     {
          std::cout.width( cout_width_count );
          std::cout << std::to_string(it);
     }
     std::cout << std::endl;
}

void print_vec( const std::vector<std::vector<float> >& vec )
{
     nir_log::info( "Start print_vec" );
     for( auto it : vec )
     {
          printv( it );
     }
     std::cout << "\n";
     nir_log::info( "End print_vec" );
}

void print_Mat( cv::Mat padded )
{
     nir_log::info( "Start print_Mat" );
     for( int i = 0; i < padded.rows; i++ )
     {
          for( int j = 0; j < padded.cols; j++ )
          {
               std::cout.width( cout_width_count );
               std::cout << std::to_string( padded.at<float>( i, j ));
          }
          std::cout << std::endl;
     }
     std::cout << "\n";
     nir_log::info( "End print_Mat" );
}

void vecToMat( const std::vector<std::vector<float> >& img, const std::vector<std::vector<float> >& re, cv::Mat& ret  )
{
     nir_log::info( "Start vecToMat" );
     ret = cv::Mat( img.size(), img[ 0 ].size(), 13 );
     for( int i = 0; i < img.size(); i++ )
     {
          for( int j = 0; j < img[ i ].size(); j++ )
          {
               ret.at<float>( i, j ) = img[ i ][ j ];
               ret.at<float>( i, j, 0 ) = re[ i ][ j ];
          }
     }
     nir_log::info( "End vecToMat" );
}

std::vector<std::vector<float>> matTovec( cv::Mat& img )
{
     nir_log::info( "Start matTovec" );
     std::vector<std::vector<float>> ret;
     for( int i = 0; i < img.cols; i++ )
     {
          ret.push_back(std::vector<float>());
          for( int j = 0; j < img.rows; j++ )
          {
               ret[i].push_back(img.at<float>( i, j ));
          }
     }
     nir_log::info( "End matTovec" );
     return ret;
}

float count_area_width( float message_length, float blocks_count )
{
     nir_log::info( "Start count_area_width" );
     float ret = ( M_PI * message_length ) / ( 44 * blocks_count );
     std::cout << "Area width( epsilon ) = " << ret << std::endl;
     nir_log::info( "End count_area_width" );
     return ret > M_PI / 2 ? -1 : ret;
}

//todo test it
float calculate_psnr( std::vector<std::vector<float> >& prev_img, std::vector<std::vector<float> >& modified_img )
{
     nir_log::info( "Start calculate_psnr" );
     nir_log::info( "End calculate_psnr" );
     return 20 * log10( find_max_img_elem( prev_img ) / calculate_rmse( prev_img, modified_img ) );
}

std::vector<float> string_to_vec_float( const std::string& message )
{
     std::vector<float> ret( message.length() );
     std::string::size_type sz;
     for( unsigned int i = 0; i < message.length(); ++i )
     {
          ret[i] = (float)message[i]  - 48;
     }
     return ret;
}

std::string preapare_substr( const std::string& str, const int& size )
{
     return str.substr(0,size);
}

std::string str_to_bits( const std::string& message )
{
     std::string ans;
     for( auto it : message )
     {
          std::bitset<8> b(it);
          ans += b.to_string();
     }
     return ans;
}

} // namespace nir_misc
