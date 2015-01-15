// debuging.h

#ifndef DEBUGING_H
#define DEBUGING_H

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define DEBUG_LOG printf
#define DEBUG_STREAM cout
#define ERROR_LOG printf
#define WRITE_VIEWED_IMG false

namespace cv_debuging {
	int viewImage(Mat frame,string name="debuging");
	int printMinMax(Mat image);
	int viewSegmentedImage(Mat frame,vector < std::vector<cv::Point2i > > &blobs,string name="Segmented image");
} // debuging

#endif