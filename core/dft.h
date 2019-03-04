#pragma once
#include <vector>
#include <opencv2/core/core.hpp>

using vecImg = std::vector<std::vector<float> >;
class nir_dft
{
public:
     nir_dft( const vecImg& im );
     nir_dft( const std::vector<std::vector<int>>& im );
     nir_dft( const cv::Mat& im );
     vecImg calc_phase( const vecImg& im, const vecImg& re ) const;
     vecImg calc_amp( const vecImg& im, const vecImg& re ) const;
     vecImg im() const;
     vecImg re() const;
     vecImg complex() const;
     vecImg do_dft() const;
     static vecImg do_dft( const vecImg& phase, const vecImg& ampl, const vecImg& im );
     vecImg get_img() const
     {
          return img_;
     };
private:
     float re_( const float x, const float y ) const;
     float im_( const float x, const float y ) const;
protected:
     vecImg img_;
};
