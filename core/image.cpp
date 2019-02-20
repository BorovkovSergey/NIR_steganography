#include "image.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <iostream>
using namespace cv;

nir_img::nir_img( char* img__name, int type )
     : img_( imread( img__name, type ) )
{
}

nir_img::nir_img( Mat& img )
     : img_( img )
{
}

Mat nir_img::DFT() const
{
     Mat padded;
     int m = getOptimalDFTSize( img_.rows );
     int n = getOptimalDFTSize( img_.cols ); // on the border add zero values
     copyMakeBorder( img_, padded, 0, m - img_.rows, 0, n - img_.cols, BORDER_CONSTANT, Scalar::all( 0 ) );

     Mat planes[] = {Mat_<float>( padded ), Mat::zeros( padded.size(), CV_32F )};
     Mat complexI;
     merge( planes, 2, complexI ); // Add to the expanded another plane with zeros

     dft( complexI, complexI ); // this way the result may fit in the source matrix

     // compute the magnitude and switch to logarithmic scale
     // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
     split( complexI, planes ); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

     magnitude( planes[ 0 ], planes[ 1 ], planes[ 0 ] ); // planes[0] = magnitude
     Mat magI = planes[ 0 ];

     magI += Scalar::all( 1 ); // switch to logarithmic scale
     log( magI, magI );

     // crop the spectrum, if it has an odd number of rows or columns
     magI = magI( Rect( 0, 0, magI.cols & - 2, magI.rows & - 2 ) );

     // rearrange the quadrants of Fourier image  so that the origin is at the image center
     int cx = magI.cols / 2;
     int cy = magI.rows / 2;

     Mat q0( magI, Rect( 0, 0, cx, cy ) );   // Top-Left - Create a ROI per quadrant
     Mat q1( magI, Rect( cx, 0, cx, cy ) );  // Top-Right
     Mat q2( magI, Rect( 0, cy, cx, cy ) );  // Bottom-Left
     Mat q3( magI, Rect( cx, cy, cx, cy ) ); // Bottom-Right

     Mat tmp; // swap quadrants (Top-Left with Bottom-Right)
     q0.copyTo( tmp );
     q3.copyTo( q0 );
     tmp.copyTo( q3 );

     q1.copyTo( tmp ); // swap quadrant (Top-Right with Bottom-Left)
     q2.copyTo( q1 );
     tmp.copyTo( q2 );

     normalize( magI, magI, 0, 1, CV_MINMAX ); // Transform the matrix with float values into a
     return magI;
}

Mat nir_img::get_DFT_amplitude() const
{
     Mat padded;
     int m = getOptimalDFTSize( img_.rows );
     int n = getOptimalDFTSize( img_.cols ); // on the border add zero values
     copyMakeBorder( img_, padded, 0, m - img_.rows, 0, n - img_.cols, BORDER_CONSTANT, Scalar::all( 0 ) );

     Mat planes[] = {Mat_<float>( padded ), Mat::zeros( padded.size(), CV_32F )};
     Mat complexI;
     merge( planes, 2, complexI ); // Add to the expanded another plane with zeros

     dft( complexI, complexI ); // this way the result may fit in the source matrix

     // compute the magnitude and switch to logarithmic scale
     // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
     split( complexI, planes );                    // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
     magnitude( planes[ 0 ], planes[ 1 ], planes[ 0 ] ); // planes[0] = magnitude
     Mat magI = planes[ 0 ];

     magI += Scalar::all( 1 ); // switch to logarithmic scale
     //log(magI, magI);

     normalize( magI, magI, 0, 255, CV_MINMAX ); // Transform the matrix with float values into a

     return magI;
}
Mat nir_img::get_DFT_phase() const
{
     Mat padded;
     int m = getOptimalDFTSize( img_.rows );
     int n = getOptimalDFTSize( img_.cols ); // on the border add zero values
     copyMakeBorder( img_, padded, 0, m - img_.rows, 0, n - img_.cols, BORDER_CONSTANT, Scalar::all( 0 ) );

     Mat planes[] = {Mat_<float>( padded ), Mat::zeros( padded.size(), CV_32F )};
     
     Mat complexI;
     merge( planes, 2, complexI ); // Add to the expanded another plane with zeros

     dft( complexI, complexI ); // this way the result may fit in the source matrix

     // compute the magnitude and switch to logarithmic scale
     // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
     split( complexI, planes ); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
     for (int i = 0; i < padded.rows - (m - img_.rows); i++)
     {
          for( int j = 0; j < padded.cols - ( n - img_.cols ); j++ )
          {
               padded.at<float>( i, j ) = atan( planes[ 1 ].at<float>( i, j ) / planes[ 0 ].at<float>( i, j ) );
          }
     }

     return padded;
}

cv::Mat nir_img::get_img() const
{
     return img_;
}
