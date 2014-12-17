// arrow_processing.hpp


#ifndef ARROW_PROCESSING_H
#define ARROW_PROCESSING_H

#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "debuging.hpp"

using namespace cv;

namespace arrow_detection {
	int preProcessing(Mat frame,Mat& pro_image,double thresh=-1,Size ksize=Size(11,11));
	int segmentation(Mat frame);
	int featureExteraction(Mat arrow);
	int otsuThresholding(Mat frame,float& thresh);
} // arrow_detection

#endif