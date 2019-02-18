#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
enum
{
     BLUE = 0,
     GREEN = 1,
     RED = 2
};
void printMAT(Mat& padded)
{
     // for( int i = 0; i < padded.rows; i++ )
     // {

     //      for( int j = 0; j < padded.cols; j++ )
     //      {
     //           std::cout << +padded.at<float>(i, j) << "\t";
     //       }
     //      std::cout << "\n";
     // }
     // std::cout << "\n" << "\n" << "\n";
}
void foo( Mat& img )
{
     int M = getOptimalDFTSize( img.rows );
     int N = getOptimalDFTSize( img.cols );
     Mat padded;
     copyMakeBorder( img, padded, 0, M - img.rows, 0, N - img.cols, BORDER_CONSTANT, Scalar::all( 0 ) );

     Mat planes[] = {Mat_<float>( padded ), Mat::zeros( padded.size(), CV_32F )};

     printMAT(padded); // todo del

     merge( planes, 2, padded );

     printMAT(padded); // todo del
     dft( padded, padded );

     printMAT(padded); // todo del

     // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))
     split( padded, planes );
     printMAT(padded); // todo del
     magnitude( planes[ 0 ], planes[ 1 ], planes[ 0 ] );
     printMAT(padded); // todo del
     padded = planes[ 0 ];
     printMAT(padded); // todo del
     padded += Scalar::all( 1 );
     printMAT(padded); // todo del
     log( padded, padded );
     printMAT(padded); // todo del

     // crop the spectrum, if it has an odd number of rows or columns
     padded = padded( Rect( 0, 0, padded.cols & - 2, padded.rows & - 2 ) );
     printMAT(padded); // todo del

     int cx = padded.cols / 2;
     int cy = padded.rows / 2;

     // rearrange the quadrants of Fourier image
     // so that the origin is at the image center
     Mat tmp;
     Mat q0( padded, Rect( 0, 0, cx, cy ) );
     Mat q1( padded, Rect( cx, 0, cx, cy ) );
     Mat q2( padded, Rect( 0, cy, cx, cy ) );
     Mat q3( padded, Rect( cx, cy, cx, cy ) );

     q0.copyTo( tmp );
     q3.copyTo( q0 );
     tmp.copyTo( q3 );

     q1.copyTo( tmp );
     q2.copyTo( q1 );
     tmp.copyTo( q2 );
     printMAT(tmp); // todo del

          printMAT(padded); // todo del
normalize( padded, padded, 0, 1, CV_MINMAX );
     padded.copyTo( img );
}
void test()
{
     Mat small_mat = Mat( 8, 8, 0 );
     small_mat.at<uchar>(0, 0) =129;
     small_mat.at<uchar>(0, 1) =128;
     small_mat.at<uchar>(0, 2) =131;
     small_mat.at<uchar>(0, 3) =129; 
     small_mat.at<uchar>(0, 4) =133;
     small_mat.at<uchar>(0, 5) =126;
     small_mat.at<uchar>(0, 6) =130;
     small_mat.at<uchar>(0, 7) =130;

     small_mat.at<uchar>(1, 0) =133;
     small_mat.at<uchar>(1, 1) =138;
     small_mat.at<uchar>(1, 2) =140;
     small_mat.at<uchar>(1, 3) =134; 
     small_mat.at<uchar>(1, 4) =136;
     small_mat.at<uchar>(1, 5) =131;
     small_mat.at<uchar>(1, 6) =134;
     small_mat.at<uchar>(1, 7) =133;

     small_mat.at<uchar>(2, 0) =136;
     small_mat.at<uchar>(2, 1) =136;
     small_mat.at<uchar>(2, 2) =128;
     small_mat.at<uchar>(2, 3) =127; 
     small_mat.at<uchar>(2, 4) =129;
     small_mat.at<uchar>(2, 5) =135;
     small_mat.at<uchar>(2, 6) =139;
     small_mat.at<uchar>(2, 7) =131;

     small_mat.at<uchar>(3, 0) =124;
     small_mat.at<uchar>(3, 1) =126;
     small_mat.at<uchar>(3, 2) =125;
     small_mat.at<uchar>(3, 3) =122; 
     small_mat.at<uchar>(3, 4) =124;
     small_mat.at<uchar>(3, 5) =123;
     small_mat.at<uchar>(3, 6) =130;
     small_mat.at<uchar>(3, 7) =123;

     small_mat.at<uchar>(4, 0) =128;
     small_mat.at<uchar>(4, 1) =126;
     small_mat.at<uchar>(4, 2) =131;
     small_mat.at<uchar>(4, 3) =126; 
     small_mat.at<uchar>(4, 4) =125;
     small_mat.at<uchar>(4, 5) =121;
     small_mat.at<uchar>(4, 6) =123;
     small_mat.at<uchar>(4, 7) =125;

     small_mat.at<uchar>(5, 0) =129;
     small_mat.at<uchar>(5, 1) =128;
     small_mat.at<uchar>(5, 2) =123;
     small_mat.at<uchar>(5, 3) =118; 
     small_mat.at<uchar>(5, 4) =119;
     small_mat.at<uchar>(5, 5) =119;
     small_mat.at<uchar>(5, 6) =121;
     small_mat.at<uchar>(5, 7) =124;

     small_mat.at<uchar>(6, 0) =118;
     small_mat.at<uchar>(6, 1) =129;
     small_mat.at<uchar>(6, 2) =125;
     small_mat.at<uchar>(6, 3) =118; 
     small_mat.at<uchar>(6, 4) =122;
     small_mat.at<uchar>(6, 5) =117;
     small_mat.at<uchar>(6, 6) =113;
     small_mat.at<uchar>(6, 7) =113;

     small_mat.at<uchar>(7, 0) =108;
     small_mat.at<uchar>(7, 1) =113;
     small_mat.at<uchar>(7, 2) =119;
     small_mat.at<uchar>(7, 3) =121; 
     small_mat.at<uchar>(7, 4) =129;
     small_mat.at<uchar>(7, 5) =120;
     small_mat.at<uchar>(7, 6) =112;
     small_mat.at<uchar>(7, 7) =110;
     foo(small_mat);
      for( int i = 0; i < small_mat.rows; i++ )
     {

          for( int j = 0; j < small_mat.cols; j++ )
          {
               std::cout << +small_mat.at<float>(i, j) << "\t";
           }
          std::cout << "\n";
     }
}


int main()
{
     int small_i, small_j;
     char* hai = "Lenna1.jpeg";
     Mat im = imread( hai, 0 );
     Mat small_mat = Mat( 8, 8, 0 );
     Mat Result;
     if( im.empty() )
     {
          cout << "Cannot load image!" << endl;
          return -1;
     }
     foo(im);  
     // test();
     // return 0;
     // // access pixel
     // for( int i = 0; i < im.rows; i++ )
     // {
     //      if ( 8 <= small_i )
     //      {
     //           small_i = 0;
     //      }
     //      for( int j = 0; j < im.cols; j++ )
     //      {
     //           small_mat.at<uchar>(small_i, small_j) = im.at<uchar>(i, j);
     //           int v = im.at<uchar>(i, j);
     //           std::cout << v << " ";
     //           ++small_j;
     //      }
     //      ++small_i;
     //      std::cout << "\n";
     // }
     imshow( "Image", im );
     waitKey( 0 );
     return 0;
}
