#pragma once
#include <vector>
#include <opencv2/core/core.hpp>

namespace nir_misc
{
/*
     @brief function for print std::vector<std::vectro<float>>
*/
void print_vec( std::vector<std::vector<float> > padded );

/*
     @brief function for print cv::Mat matrix
*/
void print_Mat( cv::Mat padded );

/*
     @brief: convert std::vector< std::vector<float> to cv::Mat
*/
void vecToMat( const std::vector<std::vector<float> >& im, const std::vector<std::vector<float> >& re, cv::Mat& ret );

/*
     @brief: function for count 'epsilon'
     @ret: -1 if ret_value < pi/2 else 'epsilon'
*/
float count_area_width( float message_length, float blocks_count );
} // namespace nir_misc
