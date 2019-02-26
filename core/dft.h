#pragma once
#include <vector>
#include <opencv2/core/core.hpp>

using vecImg = std::vector<std::vector<float> >;
class nir_dft
{
public:
     nir_dft( vecImg& im );
     nir_dft( cv::Mat& im );
     vecImg calc_phase( vecImg im, vecImg re ) const;
     vecImg calc_amp( vecImg im, vecImg re ) const;
     vecImg im() const;
     vecImg re() const;
     vecImg complex() const;
     vecImg do_dft() const;
     vecImg do_dft( vecImg phase_, vecImg ampl_, vecImg im ) const;
private:
     float re_( float x, float y ) const;
     float im_( float x, float y ) const;
protected:
     vecImg img_;
};
