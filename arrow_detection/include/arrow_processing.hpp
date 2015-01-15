// arrow_processing.hpp


#ifndef ARROW_PROCESSING_H
#define ARROW_PROCESSING_H

#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "debuging.hpp"

using namespace cv;
using namespace cv_debuging;

namespace arrow_detection {
	int preProcessing(Mat frame,Mat& pro_image,double thresh=-1,Size filterSize=Size(51,51));
	int segmentation(Mat binary_image,vector<Rect> &objects, vector <vector<Point2i>> &blobs);
	int featureExteraction(Mat arrow,vector<float> &features);
	int findBlobs(const Mat &binary, vector <vector<Point2i>> &blobs,int min_pixel_count=3500);
} // arrow_detection

#endif