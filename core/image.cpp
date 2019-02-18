#include "image.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

nir_img::nir_img( char* img_name )
     : img_( imread( img_name ) )
{
}

void nir_img::DFT( Mat& img )
{
     int M = getOptimalDFTSize( img.rows );
     int N = getOptimalDFTSize( img.cols );
     Mat padded;
     copyMakeBorder( img, padded, 0, M - img.rows, 0, N - img.cols, BORDER_CONSTANT, Scalar::all( 0 ) );

     Mat planes[] = {Mat_<float>( padded ), Mat::zeros( padded.size(), CV_32F )};

     merge( planes, 2, padded );

     dft( padded, padded );

     // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))
     split( padded, planes );
     magnitude( planes[ 0 ], planes[ 1 ], planes[ 0 ] );
     padded = planes[ 0 ];
     padded += Scalar::all( 1 );
     log( padded, padded );

     // crop the spectrum, if it has an odd number of rows or columns
     padded = padded( Rect( 0, 0, padded.cols & - 2, padded.rows & - 2 ) );

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

     normalize( padded, padded, 0, 1, CV_MINMAX );
     normalize( padded, padded, 0, 255, CV_MINMAX );
     padded.copyTo( img );
}
