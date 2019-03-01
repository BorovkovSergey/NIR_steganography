#pragma once
#include <vector>
#include <opencv2/core/core.hpp>

using vecImg = std::vector<std::vector<float> >;
class nir_dft
{
public:
     nir_dft( const vecImg& im );
     nir_dft( const cv::Mat& im );
     vecImg calc_phase( const vecImg& im, const vecImg& re ) const;
     vecImg calc_amp( const vecImg& im, const vecImg& re ) const;
     vecImg im() const;
     vecImg re() const;
     vecImg complex() const;
     vecImg do_dft() const;
     vecImg do_dft( const vecImg& phase_, const vecImg& ampl_, const vecImg& im ) const;
private:
     float re_( const float x, const float y ) const;
     float im_( const float x, const float y ) const;
protected:
     vecImg img_;
};
