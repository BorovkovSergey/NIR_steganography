#pragma once
#include <utility>
#include <vector>
#include <iostream>

#include "cv_dft.h"

class nir_cv_emb : nir_cv_dft
{
public:
     nir_cv_emb( cv::Mat& input, const float& input_area_width, const std::vector<float>& input_message );
     nir_cv_emb( cv::Mat& input, const float& input_area_width );

     void get_phase( cv::Mat& out )
     {
          out = phase;
     }
          void get_img( cv::Mat& out )
     {
          out = img;
     }

     float average_count;
     const float area_width;
     std::vector<float> auxiliary_sequence;
     std::vector<std::pair<int, int> > area_positions;
     int capacity;
     int clear_sequence_count;
     std::vector<std::vector<float> > clear_sequence;
     int overlay_options;
     float quality_characteristics;
     std::vector<float> message;
     std::vector<float> best_embedding;
     cv::Mat new_re;
     cv::Mat new_phase;
     cv::Mat new_img;
     cv::Mat new_dft;
     cv::Mat idft;
     bool is_embedded;
private:
     void create_area_positions();
     void get_average_count();
     void count_auxiliary_sequence();
     void find_clear_sequence();
     void calculate_overlay_options();
     void calculate_quality_characteristics();
     void do_new_re();
     void find_best_embedding();
     void create_new_phase_matrix( const std::vector<float>& new_values );
     bool do_test_embedded(  cv::Mat phase_to_input );
     void do_idft( cv::Mat& input );
     float get_random_phase();
};
