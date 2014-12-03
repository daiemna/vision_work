// image_stat.hpp
// Daiem Ali
// 23/10/14

#ifndef IMAGE_STAT_HPP
#define IMAGE_STAT_HPP

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

// #include "debuging.h"

using namespace cv;
using namespace std;

#define WINDOW_DIMENSION 9
namespace image_stat {

	#define ERROR_IMAGE_DATA -44 
	#define ERROR_IMAGE_MISALIGNED -14

	int SSIM(Mat image_1, Mat image_2,float *ssim);
	int SSIM_window_wise(Mat image_1, Mat image_2,float *ssim);
	int mutual_information(Mat image_1,Mat image_2,float *mi);
	int cross_correlation(Mat image_1,Mat image_2,float *corr);
	
} // image_stat


#endif // IMAGE_STAT_HPP
