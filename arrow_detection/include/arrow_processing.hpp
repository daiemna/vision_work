// arrow_processing.hpp


#ifndef ARROW_PROCESSING_H
#define ARROW_PROCESSING_H

#include <stdio.h>
#include <opencv2/opencv.hpp>
#define DEBUG_LOG printf

using namespace cv;

namespace arrow_detection {
	int preProcessing(Mat frame,Mat pro_image,double thresh);
	int segmentation(Mat frame);
	int featureExteraction(Mat arrow);
} // arrow_detection

#endif