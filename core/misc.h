#pragma once
#include <vector>
#include <opencv2/core/core.hpp>

namespace nir_misc
{
/*
     @brief function for print std::vector<std::vectro<float>>
*/

void print_vec( const std::vector<std::vector<float> >& vec );

template <class T>
void printv( const std::vector<T>& vec );

/*
     @brief function for print cv::Mat matrix
*/
void print_Mat( cv::Mat padded );

/*
     @brief: convert std::vector< std::vector<float> to cv::Mat
*/
void vecToMat( const std::vector<std::vector<float> >& img, const std::vector<std::vector<float> >& re, cv::Mat& ret  );
std::vector<std::vector<float>> matTovec( cv::Mat& img );

/*
     @brief: function for count 'epsilon'
     @ret: -1 if ret_value < pi/2 else 'epsilon'
*/
float count_area_width( float message_length, float blocks_count );

float calculate_psnr( std::vector<std::vector<float> >& prev_img, std::vector<std::vector<float> >& modified_img );

std::vector<float> string_to_vec_float( const std::string& message );
 
// dont forget that str will erase
std::string preapare_substr( const std::string& str, const int& size );

std::string str_to_bits( const std::string& message );
} // namespace nir_misc
