#include "cv_emb.h"
#include "misc.h"
#include "log.h"

#include <algorithm>
namespace
{
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
          std::cout << "WARNING: prev_vec.size() != buf.size()"; // todo fixme
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

std::vector<float> count_new_auxiliary_sequence( cv::Mat& inverseTransform, cv::Mat& new_img )
{
     nir_cv_dft img = nir_cv_dft( inverseTransform );
     std::cout << "NEW_MAT\n";
     nir_misc::print_Mat( img.img );
     nir_cv_emb emb = nir_cv_emb( img.img, 1.1424 );
     img.img.convertTo( new_img, CV_32F );
     emb.get_phase( new_img );
     return emb.auxiliary_sequence;
}

bool match_sequences( std::vector<float> message, std::vector<float> modified_img )
{
     //todo check size
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
     std::cout << std::endl;
     std::cout << std::endl;

     std::cout << "MATCH SEQUENCES" << std::endl;
     nir_misc::printv( buf_modified_msg );
     std::cout << std::endl;
     nir_misc::printv( buf_modified_img );
     std::cout << std::endl;
     std::cout << std::endl;

     return buf_modified_msg == buf_modified_img ? true : false;
}

} // namespace
nir_cv_emb::nir_cv_emb( cv::Mat& input, const float& input_area_width, std::vector<float>& input_message )
     : nir_cv_dft( input ),
     area_positions( 22 ),
     average_count( 0 ),
     area_width( input_area_width ),
     overlay_options( 1 ),
     message( input_message )
{
     phase.convertTo( new_img, CV_32F );
     phase.convertTo( new_phase, CV_32F );
     create_area_positions();
     get_average_count();
     count_auxiliary_sequence();
     find_clear_sequence();
     calculate_overlay_options();
     find_best_embedding();
     //  is_embedded = do_test_embedded( phase );
     std::vector<cv::Mat> old_value;
      do_test_embedded( new_img );
     for (int i = 0; i < 100; ++i) // 1 )
     {
         //  if ( std::find( old_value.begin(), old_value.end(), new_img ) != old_value.end() )
         //  {
         //      break;
         //  }
         old_value.push_back(new_img);
         if (do_test_embedded(new_img))
         {
             std::cout << "FOOOOOOOOOOOOOOOOOOOOOOOUUUUUUUUUUUUUUUUUUUUUUUUUUNNNNNNNNNNNNNNNNNNNNNNNNNND\n\n\n\n\n\n"
                       << std::endl;
             break;
         }
                   int a;
         std::cin >>a;
     }
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
     for( auto it : area_positions )
     {
          std::cout << average_count << std::endl;

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
     for( auto it : clear_sequence )
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

void nir_cv_emb::find_best_embedding()
{
     nir_log::info( "Start find_best_embedding" );
     std::vector<float> buf( message );
     while( auxiliary_sequence.size() > buf.size() )
     {
          buf.push_back( -1 );
     }
     std::vector<float> ret;
     int capacity = INT_MAX;
     int replacements_counter = INT_MAX;
     int buf_replacements_counter = 0;
     int buf_capacity = 0;
     while( buf.size() - 1 >= std::count( buf.begin(), buf.end(), -1 ) )
     {
          buf_capacity = best_embedding.size() - std::count( best_embedding.begin(), best_embedding.end(), -1 );
          buf_replacements_counter = calculate_replacements( auxiliary_sequence, buf );
          if( replacements_counter > buf_replacements_counter ||
              ( replacements_counter >= buf_replacements_counter && capacity < buf_capacity ) )
          {
               best_embedding = buf;
            //    for( int i = 0; i < 100; ++i )
            //    {
            //         if( do_test_embedded( new_img ) )
            //         {
            //              std::cout << "FOOOOOOOOOOOOOOOOOOOOOOOUUUUUUUUUUUUUUUUUUUUUUUUUUNNNNNNNNNNNNNNNNNNNNNNNNNND\n\n\n\n\n\n"
            //                        << std::endl;
                         replacements_counter = buf_replacements_counter;
                         capacity = buf_capacity;
            //              break;
            //         }
            //    }
          }
          next_step( message, buf );
     }

     buf_replacements_counter = calculate_replacements( auxiliary_sequence, buf );
     if( replacements_counter > buf_replacements_counter )
     {
          replacements_counter = buf_replacements_counter;
          buf_replacements_counter = 0;
          best_embedding = buf;
     }
     std::cout << "replacements_counter = " << replacements_counter << std::endl
               << "capacity = " << capacity << std::endl;
     nir_log::info( "End find_best_embedding" );
}

void nir_cv_emb::create_new_phase_matrix( const std::vector<float>& new_values )
{
     for( size_t i = 0; i < area_positions.size(); ++i )
     {
            if (new_values[i] == -1)
            {
                 continue;
            }
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

float ire_( const cv::Mat& dft, const float x, const float y )
{
     float ans = 0;
     for( int i = 0; i < dft.cols; i++ )
     {
          for( int j = 0; j < dft.rows; j++ )
          {
               ans += dft.at<float>( i, j ) * cosf32( ( i * x / 8 + j * y / 8 ) * M_PI * 2 );
          }
     }
     return ans;
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
     std::cout << "     nir_misc::print_Mat( phase_to_input )" << std::endl;
     nir_misc::print_Mat( phase_to_input );

     //    best_embedding = {-1, 0, 1, 1, -1, 1, 1, 1, 0, -1, 0, -1, -1, -1, 0, 0, 1, 1, 1, -1, 0, 1};
     //  best_embedding = {-1, 0, 1, 0, -1, 0, 0, 0, 0, -1, 1, -1, -1, -1, 0, 1, 0, 1, 1, -1, 0, 1};
     //   best_embedding = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
     //  best_embedding = { -1,0,1,1,-1,1,1,1,0,-1,0,-1,-1,-1,0,0,1,1,1,-1,0,1};
     std::vector<float> ret;
     std::cout << " au " << auxiliary_sequence.size() << " be " << best_embedding.size() << std::endl;
     if( auxiliary_sequence.size() != best_embedding.size() )
     {
          nir_log::warning( "prev.size() != best_embedding.size() in function do_test_embedded" );
     }
     for( size_t i = 0; i < auxiliary_sequence.size() && i < best_embedding.size(); ++i )
     {
        //   if( auxiliary_sequence[ i ] == best_embedding[ i ] )
        //   {
        //        ret.push_back( phase_to_input.at<float>( area_positions[ i ].second, area_positions[ i ].first ) );
        //        continue;
        //   }
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
            ret.push_back(-1);
        //   ret.push_back( phase_to_input.at<float>( area_positions[ i ].second, area_positions[ i ].first ) ); //[ area_positions_[ a ].second ][ area_positions_[ b ].first ]);//
     }
     create_new_phase_matrix( ret );
     std::cout << " best_embedding " << std::endl;
     nir_misc::printv( best_embedding );
     std::cout << " input sequence " << std::endl;
     nir_misc::printv( ret );
     std::cout << " new phase " << std::endl;
     nir_misc::print_Mat( new_phase );
     rounding( amp );
     rounding( new_phase );
     std::vector<float> buf;
     do_dft( new_phase, amp, new_dft );
     do_idft( new_dft );

     buf = count_new_auxiliary_sequence( idft, new_img );

     std::cout << "qwery\n";
     nir_misc::printv( buf );
     std::cout << "     nir_misc::print_Mat( new_phase )" << std::endl;
     nir_misc::print_Mat( new_phase );
     std::cout << "     nir_misc::print_Mat( phase_to_input )" << std::endl;
     nir_misc::print_Mat( phase_to_input );
     std::cout << "     nir_misc::print_Mat( new_img )" << std::endl;
     nir_misc::print_Mat( new_img );

     nir_log::info( "End do_test_embedded" );
     return match_sequences( best_embedding, buf );
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
