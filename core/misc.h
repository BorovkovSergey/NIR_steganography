#pragma once
#include <vector>
#include <opencv2/core/core.hpp>

namespace nir_misc
{
void print_vec( std::vector<std::vector<float> > padded );
void print_Mat( cv::Mat padded ); 
void vecToMat( const std::vector<std::vector<float> >& im, const std::vector<std::vector<float> >& re, cv::Mat& ret );
}