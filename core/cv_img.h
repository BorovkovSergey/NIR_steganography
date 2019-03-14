#pragma once

#include <vector>
#include <string>
#include "cv_emb.h"

class nir_cv_img
{
public:
     nir_cv_img( const std::string& input );
     cv::Mat img;
     cv::Mat out_img;
     std::vector<cv::Mat> vec_img;
     std::vector<cv::Mat> vec_out;
     void embedded_message( int& img_it, const float& area_width, const std::vector<float>& message );
     void do_new_img();
private:
     void do_blocks();
};
