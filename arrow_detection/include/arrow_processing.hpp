// arrow_processing.hpp


#ifndef ARROW_PROCESSING_H
#define ARROW_PROCESSING_H

#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "debuging.hpp"

using namespace cv;
using namespace cv_debuging;

namespace arrow_detection {
	int preProcessing(Mat frame,Mat& pro_image,double thresh=-1,Size ksize=Size(21,21));
	int segmentation(Mat frame);
	int featureExteraction(Mat arrow);
	int otsuThresholding(Mat frame,float& thresh);
	int findBlobs(const Mat &binary, vector <vector<Point2i>> &blobs,int min_pixel_count=0);
} // arrow_detection

#endif