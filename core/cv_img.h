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
     void do_new_img();
private:
     void do_blocks();
};
