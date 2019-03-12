#pragma once
#include <opencv2/core/core.hpp>

using vecImg = std::vector<std::vector<float> >;
class nir_cv_dft
{
public:
     nir_cv_dft( const cv::Mat& input );

     static void do_dft( const cv::Mat& new_phase, const cv::Mat& amplitude, cv::Mat& out );

     cv::Mat re;
     cv::Mat im;
     cv::Mat img;
     cv::Mat phase;
     cv::Mat amp;
     cv::Mat dft;
     cv::Mat real;
private:
     void do_dft();
     void calc_phase();
     void get_im_re();
     void calc_amp();
     float re_( const float x, const float y );
     float im_( const float x, const float y );
     void do_real();
protected:
};
