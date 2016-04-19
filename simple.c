#include <stdio.h>
#include <opencv.hpp>
#include <math.h>

using namespace cv;

const int alpha_slider_max1 = 100;
const int alpha_slider_max2 = 300;
const int alpha_slider_max3 = 46;
const int alpha_slider_max4 = 50;
const int alpha_slider_max5 = 255;

int alpha_slider1;
int alpha_slider2;
int alpha_slider3;
int alpha_slider4;
int alpha_slider5;

double cConst;
double xConst;

Mat image;
Mat new_image1;
Mat new_image2;
Mat new_image3;
Mat new_image4;
Mat new_image5;
Mat new_image6;

void on_trackbar_exp_Cfixed( int, void* )
{
  xConst = (double) alpha_slider1 / alpha_slider_max1 ;
  cConst = 1;

  /* Exponential C-Fixed */
  for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
	{ for( int c = 0; c < 3; c++ )
	    {
	      new_image2.at<Vec3b>(y,x)[c] =
		 saturate_cast<uchar>( cConst * ( pow(image.at<Vec3b>(y,x)[c], xConst)) );
	    }
	}
    }

  namedWindow("Exponential C-Fixed", 1);
  imshow( "Exponential C-Fixed", new_image2 );
}

void on_trackbar_logaritmical( int, void* )
{
  cConst = (double) alpha_slider2;

  /* Logaritmical */ 
  for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
	{ for( int c = 0; c < 3; c++ )
	    {
	      new_image3.at<Vec3b>(y,x)[c] =
		 saturate_cast<uchar>( cConst * ( log( 1 + image.at<Vec3b>(y,x)[c] ) ) );
	    }
	}
    }

  namedWindow("Logaritmical 1", 1);
  imshow( "Logaritmical 1", new_image3 );
}

void on_trackbar_exp_Xfixed( int, void* )
{
  cConst = (double) alpha_slider3 / alpha_slider_max3 ;
  xConst = 0.8;

  /* Exponential X-Fixed */
  for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
	{ for( int c = 0; c < 3; c++ )
	    {
	      new_image4.at<Vec3b>(y,x)[c] =
		saturate_cast<uchar>( cConst * ( pow(image.at<Vec3b>(y,x)[c], xConst )) );
	    }
	}
    }

  namedWindow("Exponential X-Fixed", 1);
  imshow( "Exponential X-Fixed", new_image4 );
}

void on_trackbar_const_Mfixed( int, void* )
{
  cConst = 127.5;
  xConst = (double) alpha_slider4 / alpha_slider_max4 ;

  /* Exponential X-Fixed */
  for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
	{ for( int c = 0; c < 3; c++ )
	    {
	      new_image5.at<Vec3b>(y,x)[c] =
		saturate_cast<uchar>( 255 * (1 / (1 + (cConst / (pow(image.at<Vec3b>(y,x)[c], xConst))))) );
	    }
	}
    }

  namedWindow("Contrast-Stretching M-Fixed", 1);
  imshow( "Contrast-Stretching M-Fixed", new_image5 );
}

void on_trackbar_const_Efixed( int, void* )
{
  xConst = 37;
  cConst = (double) alpha_slider5 / alpha_slider_max5;

  /* Exponential X-Fixed */
  for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
	{ for( int c = 0; c < 3; c++ )
	    {
	      new_image6.at<Vec3b>(y,x)[c] =
		saturate_cast<uchar>( 255 * (1 / (1 + (cConst / (pow(image.at<Vec3b>(y,x)[c], xConst))))) );
	    }
	}
    }

  namedWindow("Contrast-Stretching E-Fixed", 1);
  imshow( "Contrast-Stretching E-Fixed", new_image6 );
}

int main(int argc, char** argv )
{
  if ( argc != 2 )
    {
      printf("usage: DisplayImage.out <Image_Path>\n");
      return -1;
    }

  image = imread( argv[1], 1 );
  if ( !image.data )
    {
      printf("No image data \n");
      return -1;
    }

  new_image1 = Mat::zeros( image.size(), image.type() );
  new_image2 = Mat::zeros( image.size(), image.type() );
  new_image3 = Mat::zeros( image.size(), image.type() );
  new_image4 = Mat::zeros( image.size(), image.type() );
  new_image5 = Mat::zeros( image.size(), image.type() );
  new_image6 = Mat::zeros( image.size(), image.type() );

  for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
	{ for( int c = 0; c < 3; c++ )
	    {
	      new_image1.at<Vec3b>(y,x)[c] =
		saturate_cast<uchar>( 255 - ( image.at<Vec3b>(y,x)[c] ) );
	    }
	}
    }

  /// Create Windows
  namedWindow("Original Image", 1);
  namedWindow("Negative", 1);

  /// Show stuff
  imshow("Original Image", image);
  imshow("Negative", new_image1);

  /// Create Trackbars
  char TrackbarName1[50];
  sprintf( TrackbarName1, "Exponential C-Fixed %d", alpha_slider_max1 );
  createTrackbar( TrackbarName1, "Original Image", &alpha_slider1, alpha_slider_max1, on_trackbar_exp_Cfixed );
  on_trackbar_exp_Cfixed( alpha_slider1, 0 );

  /// Create Trackbars
  char TrackbarName2[50];
  sprintf( TrackbarName2, "Logaritmical %d", alpha_slider_max2 );
  createTrackbar( TrackbarName2, "Original Image", &alpha_slider2, alpha_slider_max2, on_trackbar_logaritmical );
  on_trackbar_logaritmical( alpha_slider2, 0 );

  /// Create Trackbars
  char TrackbarName3[50];
  sprintf( TrackbarName3, "Exponential X-Fixed %d", alpha_slider_max3 );
  createTrackbar( TrackbarName3, "Original Image", &alpha_slider3, alpha_slider_max3, on_trackbar_exp_Xfixed );
  on_trackbar_exp_Xfixed( alpha_slider3, 0 ); 

  /// Create Trackbars
  char TrackbarName4[50];
  sprintf( TrackbarName4, "Contrast-Stretching M-Fixed %d", alpha_slider_max4 );
  createTrackbar( TrackbarName4, "Original Image", &alpha_slider4, alpha_slider_max4, on_trackbar_const_Mfixed );
  on_trackbar_const_Mfixed( alpha_slider4, 0 ); 

  /// Create Trackbars
  char TrackbarName5[50];
  sprintf( TrackbarName5, "Contrast-Stretching E-Fixed %d", alpha_slider_max5 );
  createTrackbar( TrackbarName5, "Original Image", &alpha_slider5, alpha_slider_max5, on_trackbar_const_Efixed );
  on_trackbar_const_Efixed( alpha_slider5, 0 ); 

  waitKey(0);

  return 0;
}
