#include "cv_emb.h"
#include "misc.h"
#include "log.h"

#include <cstdlib>
#include <algorithm>
namespace
{
int match_sequences( std::vector<float> message, std::vector<float> modified_img )
{
     //todo check size
     int errors_count = 0;
     if( message.size() != modified_img.size() )
     {
          nir_log::warning( "message.size() != modified_img.size() in function match_sequences" );
     }
     std::vector<float> buf_modified_img, buf_modified_msg;
     for( size_t i = 0; i < modified_img.size(); ++i )
     {
          if( modified_img[ i ] != -1 )
          {
               buf_modified_img.push_back( modified_img[ i ] );
          }
          if( message[ i ] != -1 )
          {
               buf_modified_msg.push_back( message[ i ] );
          }
     }
     if( buf_modified_img.size() > buf_modified_msg.size() )
     {
          nir_log::info( "buf_modified_img.size() > buf_modified_msg.size()" );
          return -1;
     }
     for( size_t i = 0; i < buf_modified_img.size() && i < buf_modified_msg.size(); ++i )
     {
          if( buf_modified_img[ i ] != buf_modified_msg[ i ] )
          {
               --errors_count;
          }
     }
     if( errors_count < 0 )
     {
          return errors_count;
     }
     nir_misc::printv( buf_modified_img );
     nir_misc::printv( buf_modified_msg );
     if( buf_modified_img.size() < buf_modified_msg.size() )
     {
          return buf_modified_img.size();
     }
     return buf_modified_msg.size();
}

void new_count_auxiliary_sequence( const std::vector<std::pair<int, int> > area_positions, float area_width, cv::Mat phase )
{
     nir_log::info( "Start count_auxiliary_sequence" );
     const float f0 = -M_PI_2;
     std::vector<float> auxiliary_sequence;
     const float f1 = M_PI_2;
     for( size_t i = 0; i < area_positions.size(); ++i )
     {
          if( f0 - area_width < phase.at<float>( area_positions[ i ].first, area_positions[ i ].second ) &&
              f0 + area_width > phase.at<float>( area_positions[ i ].first, area_positions[ i ].second ) )
          {
               auxiliary_sequence.push_back( 0 );
               continue;
          }

          if( f1 - area_width < phase.at<float>( area_positions[ i ].first, area_positions[ i ].second ) &&
              f1 + area_width > phase.at<float>( area_positions[ i ].first, area_positions[ i ].second ) )
          {
               auxiliary_sequence.push_back( 1 );
               continue;
          }
          auxiliary_sequence.push_back( -1 );
     }
     nir_log::info( "End count_auxiliary_sequence" );
}
void rounding( cv::Mat& img )
{
     nir_log::info( "Start nir_dft rounding" );
     int buf;
     for( size_t i = 0; i < img.rows; ++i )
     {
          for( size_t j = 0; j < img.cols; ++j )
          {
               img.at<float>( i, j ) *= 1000;
               buf = img.at<float>( i, j );
               if( buf % 10 >= 5 || buf % 10 <= -5 )
               {
                    buf /= 10;
                    if( buf < 0 )
                    {
                         --buf;
                    }
                    else
                    {
                         ++buf;
                    }
                    img.at<float>( i, j ) = buf;
                    img.at<float>( i, j ) /= 100;
                    continue;
               }
               buf /= 10;
               img.at<float>( i, j ) = buf;
               img.at<float>( i, j ) /= 100;
          }
     }
     nir_log::info( "End nir_dft rounding" );
}
int calculate_replacements( const std::vector<float>& prev_vec, const std::vector<float>& buf )
{
     int ret = 0;
     if( prev_vec.size() != buf.size() )
     {
          nir_log::warning( "prev_vec.size() != buf.size() in function calculate_replacements" );
     }
     for( size_t i = 0; i < prev_vec.size() && i < buf.size(); ++i )
     {
          if( prev_vec[ i ] != buf[ i ] )
          {
               ++ret;
          }
     }
     return ret;
}

void next_step( const std::vector<float>& m_vec, std::vector<float>& buf )
{
     for( size_t i = buf.size() - 1; i >= 0; --i )
     {
          if( -1 != buf[ i ] )
          {
               buf[ i ] = -1;
               for( size_t j = i + 1; j < buf.size(); ++j )
               {
                    std::vector<float>::iterator it = buf.begin() + j;
                    buf[ j ] = m_vec[ j - std::count( buf.begin(), it, -1 ) ];
               }
               break;
          }
     }
}

std::vector<std::vector<float> > rounding( std::vector<std::vector<float> > img )
{
     nir_log::info( "Start nir_dft rounding" );
     int buf;
     for( size_t i = 0; i < img.size(); ++i )
     {
          for( size_t j = 0; j < img.size(); ++j )
          {
               img[ i ][ j ] *= 1000;
               buf = img[ i ][ j ];
               if( buf % 10 >= 5 || buf % 10 <= -5 )
               {
                    buf /= 10;
                    if( buf < 0 )
                    {
                         --buf;
                    }
                    else
                    {
                         ++buf;
                    }
                    img[ i ][ j ] = buf;
                    img[ i ][ j ] /= 100;
                    continue;
               }
               buf /= 10;
               img[ i ][ j ] = buf;
               img[ i ][ j ] /= 100;
          }
     }
     nir_log::info( "End nir_dft rounding" );
     return img;
}
void calculate_mat_items_dif( cv::Mat in, cv::Mat r_mat, std::vector<int>& rows_dif, std::vector<int>& cols_dif )
{
     float ret1 = 0, ret2 = 0;
     //  std::cout << "rows\n";
     for( unsigned int i = 0; i < in.cols; ++i )
     {
          for( unsigned j = 0; j < in.rows; ++j )
          {
               ret1 += r_mat.at<float>( i, j );
               ret2 += in.at<float>( i, j );
          }
          ret1 -= ret2;
          rows_dif.push_back( ret1 );
          ret1 = 0;
          ret2 = 0;
     }
     //  std::cout << "cols\n";
     for( unsigned int i = 0; i < in.cols; ++i )
     {
          for( unsigned j = 0; j < in.rows; ++j )
          {
               ret1 += r_mat.at<float>( j, i );
               ret2 += in.at<float>( j, i );
          }
          ret1 -= ret2;
          cols_dif.push_back( ret1 );
          ret1 = 0;
          ret2 = 0;
     }
}

// не знал как назвать мб потом поменяю
// крч смотрит разницу в колонках и строках и убирает ее
void optimize_mat( cv::Mat& r_mat, std::vector<int>& rows_dif, std::vector<int>& cols_dif )
{
     for( int i = 0; i < cols_dif.size(); ++i )
     {
          if( cols_dif[ i ] < 0 )
          {
               for( int j = 0; j < rows_dif.size(); ++j )
               {
                    if( rows_dif[ j ] < 0 )
                    {
                         ++r_mat.at<float>( j, i );
                         rows_dif[ j ]++;
                         cols_dif[ i ]++;
                         if( cols_dif[ i ] != 0 )
                         {
                              --i;
                         }
                         break;
                    }
               }
               continue;
          }
          if( cols_dif[ i ] > 0 )
          {
               bool flag = false;
               for( auto j = 0; j < rows_dif.size(); ++j )
               {
                    if( rows_dif[ j ] > 0 )
                    {
                         --r_mat.at<float>( j, i );
                         rows_dif[ j ]--;
                         cols_dif[ i ]--;
                         if( cols_dif[ i ] == 0 )
                         {
                              --i;
                         }
                         break;
                    }
               }
          }
     }
}
std::vector<float> count_new_auxiliary_sequence_fix( float area_width, const std::vector<float>& aux_seq, cv::Mat& inverseTransform, cv::Mat& new_phase )
{
     std::vector<int> rows_dif;
     std::vector<int> cols_dif;
     nir_cv_dft img = nir_cv_dft( inverseTransform );
     cv::Mat new_mat;
     img.img.convertTo( new_mat, CV_32F );

     {
          nir_cv_emb emb = nir_cv_emb( img.img, area_width );

          if( match_sequences( aux_seq, emb.auxiliary_sequence ) != -1 )
          {
               new_mat.convertTo( new_phase, CV_32F );
               emb.get_phase( new_phase );
               new_mat.convertTo( inverseTransform, CV_32F );
               return emb.auxiliary_sequence;
          }
     }

     calculate_mat_items_dif( inverseTransform, new_mat, rows_dif, cols_dif );
     optimize_mat( new_mat, rows_dif, cols_dif );

     nir_cv_emb emb = nir_cv_emb( new_mat, area_width );
     new_mat.convertTo( new_phase, CV_32F );
     emb.get_phase( new_phase );
     new_mat.convertTo( inverseTransform, CV_32F );
     return emb.auxiliary_sequence;
}

std::vector<float> count_new_auxiliary_sequence( float area_width, const std::vector<float>& aux_seq, cv::Mat& inverseTransform, cv::Mat& new_phase )
{
     std::vector<int> rows_dif;
     std::vector<int> cols_dif;
     nir_cv_dft img = nir_cv_dft( inverseTransform );
     cv::Mat new_mat;
     img.img.convertTo( new_mat, CV_32F );

     {
          nir_cv_emb emb = nir_cv_emb( img.img, area_width );

          if( match_sequences( aux_seq, emb.auxiliary_sequence ) != -1 )
          {
               new_mat.convertTo( new_phase, CV_32F );
               emb.get_phase( new_phase );
               new_mat.convertTo( inverseTransform, CV_32F );
               return emb.auxiliary_sequence;
          }
     }

     nir_cv_emb emb = nir_cv_emb( new_mat, area_width );
     new_mat.convertTo( new_phase, CV_32F );
     emb.get_phase( new_phase );
     new_mat.convertTo( inverseTransform, CV_32F );
     return emb.auxiliary_sequence;
}
std::vector<float> count_new2_auxiliary_sequence( float area_width, cv::Mat& inverseTransform, cv::Mat& new_phase )
{
     nir_cv_dft img = nir_cv_dft( inverseTransform );
     //  cv::Mat buf_img;
     //  img.img.convertTo(buf_img, CV_32F);

     nir_cv_emb emb = nir_cv_emb( img.img, area_width );
     //  std::cout << "after " << nir_misc::calculate_mat_items_sum( img.img ) << std::endl;
     img.img.convertTo( new_phase, CV_32F );
     emb.get_phase( new_phase );
     img.img.convertTo( inverseTransform, CV_32F );
     return emb.auxiliary_sequence;
}

} // namespace
nir_cv_emb::nir_cv_emb( cv::Mat& input, const float& input_area_width, const std::vector<float>& input_message, int min_capacity_i )
     : nir_cv_dft( input ),
     area_positions( 22 ),
     average_count( 0 ),
     area_width( input_area_width ),
     overlay_options( 1 ),
     message( input_message ),
     is_embedded( false ),
     min_capacity( min_capacity_i )
{
     phase.convertTo( new_img, CV_32F );
     phase.convertTo( new_phase, CV_32F );
     create_area_positions();
     get_average_count();
     count_auxiliary_sequence();
     find_clear_sequence();
     calculate_overlay_options();
     find_best_embedding();
     if( !is_embedded )
     {
          while( true )
          {
               if( std::find( auxiliary_sequence.begin(), auxiliary_sequence.end(), 0 ) != auxiliary_sequence.end() ||
                   std::find( auxiliary_sequence.begin(), auxiliary_sequence.end(), 1 ) != auxiliary_sequence.end() )
               {
                    if( std::count( auxiliary_sequence.begin(), auxiliary_sequence.end(), 1 ) + std::count( auxiliary_sequence.begin(), auxiliary_sequence.end(), 0 ) <= min_capacity )
                    {
                         break;
                    }
                    break;
                    processing_empty_block();
               }
               else
               {
                    break;
               }
          }
     }
     idft_.convertTo( new_img, CV_32F );
}

nir_cv_emb::nir_cv_emb( cv::Mat& input, const float& input_area_width )
     : nir_cv_dft( input ),
     area_positions( 22 ),
     area_width( input_area_width )
{
     create_area_positions();
     count_auxiliary_sequence();
}
void nir_cv_emb::get_average_count()
{
     nir_log::info( "Start get_average_count" );
     for( auto& it : area_positions )
     {
          average_count += phase.at<float>( it.first, it.second ) < 0 ? -phase.at<float>( it.first, it.second ) : phase.at<float>( it.first, it.second );
     }
     average_count /= area_positions.size();
     nir_log::info( "End get_average_count" );
}

void nir_cv_emb::count_auxiliary_sequence()
{
     nir_log::info( "Start count_auxiliary_sequence" );
     const float f0 = -M_PI_2;
     const float f1 = M_PI_2;
     for( size_t i = 0; i < area_positions.size(); ++i )
     {
          if( f0 - area_width < phase.at<float>( area_positions[ i ].first, area_positions[ i ].second ) &&
              f0 + area_width > phase.at<float>( area_positions[ i ].first, area_positions[ i ].second ) )
          {
               auxiliary_sequence.push_back( 0 );
               continue;
          }

          if( f1 - area_width < phase.at<float>( area_positions[ i ].first, area_positions[ i ].second ) &&
              f1 + area_width > phase.at<float>( area_positions[ i ].first, area_positions[ i ].second ) )
          {
               auxiliary_sequence.push_back( 1 );
               continue;
          }
          auxiliary_sequence.push_back( -1 );
     }
     nir_log::info( "End count_auxiliary_sequence" );
}

void nir_cv_emb::find_clear_sequence()
{
     nir_log::info( "Start find_clear_sequence" );
     clear_sequence_count = 0;
     int counter = -1;
     bool plus = false;
     bool minus = false;
     for( size_t i = 0; i < auxiliary_sequence.size(); i++ )
     {
          if( !minus && -1 == auxiliary_sequence[ i ] )
          {
               minus = true;
               ++clear_sequence_count;
               clear_sequence.push_back( std::vector<float>() );
               plus = false;
               ++counter;
          }
          if( !plus && 0 <= auxiliary_sequence[ i ] )
          {
               plus = true;
               clear_sequence.push_back( std::vector<float>() );
               minus = false;
               ++counter;
          }
          clear_sequence[ counter ].push_back( auxiliary_sequence[ i ] );
     }
     nir_log::info( "End find_clear_sequence" );
}

void nir_cv_emb::calculate_overlay_options()
{
     nir_log::info( "Start calculate_overlay_options" );
     for( auto& it : clear_sequence )
     {
          if( it[ 0 ] < 0 )
          {
               overlay_options *= it.size() + 1;
          }
     }
     nir_log::info( "End calculate_overlay_options" );
}

void nir_cv_emb::calculate_quality_characteristics()
{
     nir_log::info( "Start calculate_quality_characteristics" );
     float s = 0;
     int extra_i = 0;
     float d = 0;
     for( size_t i = 0; i + extra_i < auxiliary_sequence.size() && i < message.size(); ++i )
     {
          if( -1 == auxiliary_sequence[ i + extra_i ] )
          {
               ++extra_i;
               --i;
               continue;
          }
          if( auxiliary_sequence[ i + extra_i ] == message[ i ] )
          {
               ++s;
               continue;
          }
          ++d;
     }
     quality_characteristics = ( s - d ) / ( s + d );
     nir_log::info( "End calculate_quality_characteristics" );
}
std::vector<float> make_not_optimal_embedded_sequence( const std::vector<float>& auxiliary_sequence, const std::vector<float>& message )
{
     std::vector<float> ans;
     int counter = 0;
     for( size_t i = 0; i < auxiliary_sequence.size(); ++i )
     {
          if( auxiliary_sequence[ i ] == -1 )
          {
               ans.push_back( auxiliary_sequence[ i ] );
               continue;
          }
          ans.push_back( message[ counter ] );
          ++counter;
     }
     return ans;
}
void nir_cv_emb::find_best_embedding()
{
     nir_log::info( "Start find_best_embedding" );
     std::vector<float> buf( message );
     while( auxiliary_sequence.size() > buf.size() )
     {
          buf.push_back( -1 );
     }
     std::vector<float> not_optimal = make_not_optimal_embedded_sequence( auxiliary_sequence, message );
     int not_optimal_replacements = calculate_replacements( auxiliary_sequence, not_optimal );
     std::vector<float> ret;
     capacity = INT_MAX;
     int replacements_counter = INT_MAX;
     int buf_replacements_counter = 0;
     int buf_capacity = 0;
     is_embedded = false;

     while( buf.size() - 1 >= std::count( buf.begin(), buf.end(), -1 ) )
     {
          buf_capacity = buf.size() - std::count( buf.begin(), buf.end(), -1 );
          buf_replacements_counter = calculate_replacements( auxiliary_sequence, buf );
          if( buf_capacity > min_capacity )
          {

               if( ( replacements_counter > buf_replacements_counter && buf_replacements_counter < not_optimal_replacements ) ||
                   ( buf_replacements_counter < not_optimal_replacements && replacements_counter >= buf_replacements_counter && capacity < buf_capacity ) )
               {
                    best_embedding = buf;

                    for( int i = 0; i < 100; ++i )
                    {
                         if( do_test_embedded( new_phase ) )
                         {
                              capacity = buf_capacity_;
                              replacements_counter = buf_replacements_counter;
                              is_embedded = true;
                              std::cout << "I= " << i << std::endl;
                              idft.convertTo( idft_, CV_32F );
                              break;
                         }
                    }
               }
          }
          next_step( message, buf );
     }
     if( !is_embedded )
     {
          capacity = 0;
     }
     nir_log::info( "End find_best_embedding" );
}

void nir_cv_emb::create_new_phase_matrix( const std::vector<float>& new_values )
{
     for( size_t i = 0; i < area_positions.size(); ++i )
     {
          new_phase.at<float>( area_positions[ i ].first, area_positions[ i ].second ) = new_values[ i ];
     }
}
void nir_cv_emb::do_new_re()
{
     // todo сравнение размеров ре и фейз
     if( amp.size() != new_phase.size() )
     {
          nir_log::warning( "ampl.size() != new_phase.size() inf func do_new_re " );
     }
     re.convertTo( new_re, CV_32F );
     for( size_t i = 0; i < amp.cols && i < new_phase.cols; ++i )
     {
          for( size_t j = 0; j < amp.rows && j < new_phase.rows; ++j )
          {
               new_re.at<float>( i, j ) = ( amp.at<float>( i, j ) * cos( new_phase.at<float>( i, j ) ) );
          }
     }
}

float nir_cv_emb::get_random_phase()
{
     srand( time( NULL ) );
     const float f0 = -M_PI_2;
     const float f1 = M_PI_2;
     while( true )
     {
          float r = rand();
          int ran = rand() % 3;
          while( true )
          {
               r /= 10;
               if( r < 1 )
               {
                    break;
               }
          }
          if( f0 - area_width < ran + r &&
              f0 + area_width > ran + r )
          {
               continue;
          }

          if( f1 - area_width < ran + r &&
              f1 + area_width > ran + r )
          {
               continue;
          }
          return ran + r;
     }
}

void nir_cv_emb::do_idft( cv::Mat& input )
{
     nir_log::info( "Start do_idft" );
     cv::dft( input, idft, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT );
     nir_log::info( "End do_idft" );
}

bool nir_cv_emb::do_test_embedded( cv::Mat phase_to_input )
{
     nir_log::info( "Start do_test_embedded" );
     std::vector<float> ret;
     if( auxiliary_sequence.size() != best_embedding.size() )
     {
          nir_log::warning( "prev.size() != best_embedding.size() in function do_test_embedded" );
     }
     for( size_t i = 0; i < auxiliary_sequence.size() && i < best_embedding.size(); ++i )
     {
          if( auxiliary_sequence[ i ] == best_embedding[ i ] )
          {
               ret.push_back( phase_to_input.at<float>( area_positions[ i ].first, area_positions[ i ].second ) );
               continue;
          }
          if( best_embedding[ i ] == 0 )
          {
               ret.push_back( -M_PI_2 );
               continue;
          }
          if( best_embedding[ i ] == 1 )
          {
               ret.push_back( M_PI_2 );
               continue;
          }
          ret.push_back( 3.14 ); //get_random_phase() );
     }
     create_new_phase_matrix( ret );
     std::vector<float> buf;
     do_dft( new_phase, amp, new_dft );
     do_idft( new_dft );

     cv::Mat idft_buf;
     cv::Mat new_phase_buf;
     idft.convertTo( idft_buf, CV_32F );
     new_phase.convertTo( new_phase_buf, CV_32F );

     buf = count_new_auxiliary_sequence( area_width, best_embedding, idft_buf, new_phase_buf );
     nir_log::info( "End do_test_embedded" );
     buf_capacity_ = match_sequences( best_embedding, buf );
     if( buf_capacity_ < -999999 )
     {
          cv::Mat idft_buf_fix;
          cv::Mat new_phase_buf_fix;
          std::vector<float> buf_fix;
          idft.convertTo( idft_buf_fix, CV_32F );
          new_phase.convertTo( new_phase_buf_fix, CV_32F );
          buf_fix = count_new_auxiliary_sequence_fix( area_width, best_embedding, idft_buf_fix, new_phase_buf_fix );
          int buf_capacity_fix = match_sequences( best_embedding, buf_fix );
          if( buf_capacity_fix > buf_capacity_ )
          {
               buf_capacity_ = buf_capacity_fix;
               idft_buf_fix.convertTo( idft, CV_32F );
               new_phase_buf_fix.convertTo( new_phase, CV_32F );
               buf = buf_fix;
          }
     }
     else
     {
          idft_buf.convertTo( idft, CV_32F );
          new_phase_buf.convertTo( new_phase, CV_32F );
     }
     if( buf_capacity_ < min_capacity )
     {
          buf_capacity_ = -1;
     }
     if( 0 > buf_capacity_ )
     {
          auxiliary_sequence = buf;

          return false;
     }
     return true;
}

void nir_cv_emb::processing_empty_block()
{
     nir_log::info( "Start processing_empty_block" );
     srand( time( NULL ) );
     const float f0 = -M_PI_2;
     const float f1 = M_PI_2;
     for( size_t i = 0; i < area_positions.size(); ++i )
     {
          for( size_t j = 0; j < area_positions.size(); ++j )
          {
               {
                    if( f0 - area_width <= phase.at<float>( i, j ) &&
                        f0 + area_width >= phase.at<float>( i, j ) )
                    {
                         phase.at<float>( i, j ) = get_random_phase();
                         continue;
                    }

                    if( f1 - area_width <= phase.at<float>( i, j ) &&
                        f1 + area_width >= phase.at<float>( i, j ) )
                    {
                         phase.at<float>( i, j ) = get_random_phase();
                         continue;
                    }
               }
          }
     }
     do_dft( phase, amp, new_dft );
     do_idft( new_dft );
     auxiliary_sequence = count_new2_auxiliary_sequence( area_width, idft, new_phase );
     new_count_auxiliary_sequence( area_positions, area_width, phase );
     new_phase.convertTo( phase, CV_32F );
     idft.convertTo( idft_, CV_32F );
     nir_log::info( "End processing_empty_block" );
}
void nir_cv_emb::create_area_positions()
{
     nir_log::info( "Start create_area_positions" );
     area_positions[ 0 ].second = 3;
     area_positions[ 0 ].first = 1;
     area_positions[ 1 ].second = 4;
     area_positions[ 1 ].first = 1;
     area_positions[ 2 ].second = 2;
     area_positions[ 2 ].first = 2;
     area_positions[ 3 ].second = 3;
     area_positions[ 3 ].first = 2;
     area_positions[ 4 ].second = 4;
     area_positions[ 4 ].first = 2;
     area_positions[ 5 ].second = 1;
     area_positions[ 5 ].first = 3;
     area_positions[ 6 ].second = 2;
     area_positions[ 6 ].first = 3;
     area_positions[ 7 ].second = 3;
     area_positions[ 7 ].first = 3;
     area_positions[ 8 ].second = 4;
     area_positions[ 8 ].first = 3;
     area_positions[ 9 ].second = 1;
     area_positions[ 9 ].first = 4;
     area_positions[ 10 ].second = 2;
     area_positions[ 10 ].first = 4;
     area_positions[ 11 ].second = 3;
     area_positions[ 11 ].first = 4;
     area_positions[ 12 ].second = 4;
     area_positions[ 12 ].first = 4;
     area_positions[ 13 ].second = 1;
     area_positions[ 13 ].first = 5;
     area_positions[ 14 ].second = 2;
     area_positions[ 14 ].first = 5;
     area_positions[ 15 ].second = 3;
     area_positions[ 15 ].first = 5;
     area_positions[ 16 ].second = 1;
     area_positions[ 16 ].first = 6;
     area_positions[ 17 ].second = 2;
     area_positions[ 17 ].first = 6;
     area_positions[ 18 ].second = 3;
     area_positions[ 18 ].first = 6;
     area_positions[ 19 ].second = 1;
     area_positions[ 19 ].first = 7;
     area_positions[ 20 ].second = 2;
     area_positions[ 20 ].first = 7;
     area_positions[ 21 ].second = 3;
     area_positions[ 21 ].first = 7;
     nir_log::info( "End create_area_positions" );
}
