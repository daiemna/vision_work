// image_stat.hpp
// Daiem Ali
// 23/10/14

#ifndef IMAGE_STAT_HPP
#define IMAGE_STAT_HPP

#include <opencv2/opencv.hpp>
#include <iostream>

#include "debuging.h"

using namespace cv;
using namespace std;
namespace image_stat {

	#define ERROR_MI_IMAGE_DATA -44 
	#define ERROR_MI_IMAGE_MISALIGNED -14

	int mutual_information(Mat image_1,Mat image_2,float *mi);
	
} // image_stat


#endif // IMAGE_STAT_HPP
