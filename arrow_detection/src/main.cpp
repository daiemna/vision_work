// main.cpp
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include "arrow_processing.hpp"

using namespace arrow_detection;
using namespace cv;
using namespace std;

#define DEBUG_LOG printf
#define DEBUG_STREAM cout
#define ERROR_LOG printf


int main(int argc,char* argv[]){
	DEBUG_LOG("arrow_detection main!\n");

	if(argc < 2){
		ERROR_LOG("no image path found in arguments\n");
		return -1;
	}

	Mat frame, bin_image;
	frame = imread(argv[1],CV_LOAD_IMAGE_COLOR);
	if(!frame.data){
		ERROR_LOG("Cannot open image\n");
	}
	cvtColor(frame,frame,CV_RGB2GRAY);
	DEBUG_STREAM << "frame type: " << frame.type() << endl;

	namedWindow( "RESULT", WINDOW_NORMAL );
	int key = 0;
	double thresh = 0;
	while((key = waitKey(0)) != 1048586){
		DEBUG_STREAM << "key was : " << key << endl;
		threshold( frame, bin_image, 1, thresh,THRESH_BINARY);
		if(bin_image.data)
			imshow( "RESULT", bin_image);
		if(key == 1114027){
			thresh++;
			if(thresh > 255)
				thresh = 255;
		}else 	if(key == 1114029){
			thresh--;
			if (thresh < 0)
				thresh = 0;
		}
		DEBUG_STREAM << "Threshhold is: " << thresh << endl;
	}
	return 0;
}