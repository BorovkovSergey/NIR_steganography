#include <utility>
#include <vector>
#include <iostream>
#include "dft.h"
class nir_embedding
{
public:
     nir_embedding() = delete;
     nir_embedding( const std::vector<std::vector<float> >& phase );

     // @brief we can't do embedded if A critical < A
     // step 3.1 of algorithm from article
     bool may_we_embedding( const float& A ) const
     {
          std::cout << "Average count = " << average_count_ << std::endl
                    << "A critical = " << A << std::endl;
          return A < average_count_ ? false : true;
     }

     /*
           f[i] = 0       if phase[0] - epsilon < phaes[i] < phase[0] - epsilon
           f[i] = 1       if phase[1] - epsilon < phaes[i] < phase[1] - epsilon
           f[i] = -1      if phase[0] - epsilon < phaes[i] < phase[0] - epsilon && phase[1] - epsilon < phaes[i] < phase[1] - epsilon
           step 3.3 of algorithm from article
      */
     std::vector<float> count_auxiliary_sequence( const float& area_width ) const;

     /*
           @brief search for sequences consisting of -1
           step 3.5 of algorithm from article
      */
     std::vector<std::vector<float> > find_clear_sequence( const std::vector<float>& auxiliary_sequence );

     /*
           @brief function for get clear sequence count
           !!! don't forget use find_clear_sequence before !!!
      */
     int get_clear_sequence_count() const
     {
          return clear_sequence_count_;
     }

     /*
           @brief calculate the number of options for the imposition of the sequence M on F
           step 3.5 of algorithm from article
      */
     int calculate_overlay_options( const std::vector<std::vector<float> >& clear_sequence ) const;

     /*
           @brief calculation of quality characteristics
           step 3.6 of algorithm from article
      */
     float calculate_quality_characteristics( const std::vector<float>& M, const std::vector<float>& F ) const;

     /*
            @brief the number of derived variants is calculated, differing by the different placement of bits in empty gaps
            step 3.7.1 of algorithm from article
     */
     float calculate_different_placement( const std::vector<std::vector<float>>& clear_seq_matrix ); 
private:
     void get_average_count();
     void create_area_positions();
protected:
     std::vector<std::pair<int, int> > area_positions_;
     std::vector<std::vector<float> > phase_;
     int clear_sequence_count_;
     float average_count_;
};
