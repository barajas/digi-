#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace cv;

const string lowPassName = "Low-Pass Butterworth";

void shift_DFT(Mat& fImage)
{
  Mat tmp, q0, q1, q2, q3;

  fImage = fImage(Rect(0, 0, fImage.cols & -2, fImage.rows & -2));

  int cx = fImage.cols/2;
  int cy = fImage.rows/2;

  q0 = fImage(Rect(0, 0, cx, cy));
  q1 = fImage(Rect(cx, 0, cx, cy));
  q2 = fImage(Rect(0, cy, cx, cy));
  q3 = fImage(Rect(cx, cy, cx, cy));

  q0.copyTo(tmp);
  q3.copyTo(q0);
  tmp.copyTo(q3);

  q1.copyTo(tmp);
  q2.copyTo(q1);
  tmp.copyTo(q2);
}

int main( int argc, char** argv )
{
  Point center;
  Mat planes[2];
  Mat img, img_gray, img_out , tmp;
  Mat padding, complexI, filter;
  int R, C;
  int freq = 30;
  double radius;

  if ( argc != 2 ) {
      printf("usage: DisplayImage.out <Image_Path>\n");
      return -1;
  }

  img = imread( argv[1], 1 );
  if ( !img.data ) {
      printf("No image data \n");
      return -1;
  }  

  namedWindow(lowPassName, 0);

  /* preprocesing geting image size */
  R = getOptimalDFTSize( img.rows );
  C = getOptimalDFTSize( img.cols );

  createTrackbar("Frequency", lowPassName, &freq, (min(R, C) / 2));

  while (1) {

    /* 1. Convert the input image to class type desired if needed */
    cvtColor(img, img_gray, CV_BGR2GRAY);

    /* 2. Obtain the padding parameters */
    copyMakeBorder(img_gray, padding, 0, R - img_gray.rows, 0,
                   C - img_gray.cols, BORDER_CONSTANT, Scalar::all(0));

    planes[0] = Mat_<float>(padding);
    planes[1] = Mat::zeros(padding.size(), CV_32F);
    merge(planes, 2, complexI);

    /* 3. Obtain fourier transform with padding */
    dft(complexI, complexI);

    /* 4. Generate a filter function with same parameter padding */
    filter = complexI.clone();
    tmp = Mat(filter.rows, filter.cols, CV_32F);
    center = Point(filter.rows / 2, filter.cols / 2);

    for(int i = 0; i < filter.rows; i++)
    {
      for(int j = 0; j < filter.cols; j++)
      {
        radius = (double) sqrt(pow((i - center.x), 2.0) + 
                               pow((double) (j - center.y), 2.0));

	tmp.at<float>(i,j) =
           (float) ( 1 / (1 + pow((double) (radius / freq),
           (double) (2 * 2))));
      }
    }

    Mat toMerge[] = {tmp, tmp};
    merge(toMerge, 2, filter);

    /* Crop the top, left rectangle and get the center */
    shift_DFT(complexI);

    /* 5. Multiply the transform by the filter. */
    mulSpectrums(complexI, filter, complexI, 0);

    /* Crop the top, left rectangle and get the center */
    shift_DFT(complexI);

    /* 6. Obtain the inverse FFT of the result */
    idft(complexI, complexI);

    /* Extract the output */
    split(complexI, planes);
    normalize(planes[0], img_out, 0, 1, CV_MINMAX);

    imshow(lowPassName, img_out);

    waitKey(20);

    }

  return 0;
}
