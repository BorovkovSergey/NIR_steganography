#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <map>
#include "misc.h"
#include "dft.h"
#include "test.h"
#include <vector>
#include "log.h"

template <class T>
void printv( std::vector<T> vec )
{
     for( auto it : vec )
     {
          std::cout << it << " ";
     }
     std::cout << std::endl;
}
int calculate_replacements( const std::vector<int>& prev_vec, const std::vector<int>& buf )
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
void next_step( const std::vector<int>& m_vec, std::vector<int>& buf )
{
     for( size_t i = buf.size() - 1; i >= 0; --i )
     {
          if( -1 != buf[ i ] )
          {
               buf[ i ] = -1;
               for( size_t j = i + 1; j < buf.size(); ++j )
               {
                    std::vector<int>::iterator it = buf.begin() + j;
                    buf[j] = m_vec[ j - std::count( buf.begin(), it, -1 )];
               }
               break;
          }
     }
}
void perebor( const std::vector<int>& prev_vec, const std::vector<int>& m_vec )
{
     std::vector<int> buf( m_vec );
     std::vector<int> ret;
     int replacements_counter = INT_MAX;
     int buf_replacements_counter = 0;
     while( buf.size() - 1 >= std::count( buf.begin(), buf.end(), -1 ) )
     {
          buf_replacements_counter = calculate_replacements( prev_vec, buf );
          if( replacements_counter > buf_replacements_counter )
          {
               replacements_counter = buf_replacements_counter;
               buf_replacements_counter = 0;
               ret = buf;
          }
          next_step( m_vec, buf );
     }
     buf_replacements_counter = calculate_replacements( prev_vec, buf );
     if( replacements_counter > buf_replacements_counter )
     {
          replacements_counter = buf_replacements_counter;
          buf_replacements_counter = 0;
          ret = buf;
     }
     printv(prev_vec);
     printv(ret);
     std::cout << replacements_counter;
}
void foo()
{
     std::vector<int> testvec = {-1, -1,1,0,0,-1,-1,1,-1,1,-1,0,-1,0,-1,1,-1,1,1,0,-1,0};
     std::vector<int> input_vec = {0,1,1,1,0,1,1,1,1,0,0,0,1,0,1,1,0,0,0,0,0,0};
     perebor( testvec, input_vec );
     //  вроде работает только нужно вовремя останавливаться, а то к миниммальным изменениям сводит и теряется емкость.
     // todo перенсти в класс!!!!!!!!!!!
}
int main()
{
     foo();
          char* pic = "Lenna1.jpeg";
     // nir_test::test();
     // nir_test::test_vec();
     return 0;
}
