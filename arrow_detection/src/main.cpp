// main.cpp
#include <opencv2/opencv.hpp>
#include "arrow_processing.hpp"
#include "debuging.hpp"

using namespace arrow_detection;
using namespace cv;
using namespace std;
using namespace cv_debuging;



int test_thresholding(int argc, char* argv[]);
int test_otsu_thresholding(int argc, char* argv[]);
int test_PreProcessing(int argc, char* argv[]);
int test_blurring(int argc, char* argv[]);

int main(int argc,char* argv[]){
	DEBUG_LOG("arrow_detection main!\n");
	// int error = test_thresholding(argc,argv);
	// int error = test_otsu_thresholding(argc,argv);
	// int error = test_PreProcessing(argc,argv);
	int error = test_blurring(argc,argv);
	return 0;
}

int test_blurring(int argc, char* argv[]){
	DEBUG_LOG("Inside bluring\n!");
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
	double thresh = 3;
	while((key = waitKey(0)) != 1048586){
		DEBUG_STREAM << "key was : " << key << endl;
		preProcessing(frame, bin_image, -1, Size(thresh, thresh));
		if(bin_image.data)
			imshow( "RESULT", bin_image);
		if(key == 1114027){
			thresh+=2;
			if(thresh > 51)
				thresh = 51;
		}else 	if(key == 1114029){
			thresh-=2;
			if (thresh < 3)
				thresh = 3;
		}
		DEBUG_STREAM << "Filter Size is: " << thresh << endl;
	}
	return 0;
}

int test_PreProcessing(int argc, char* argv[]){
	DEBUG_LOG("Inside test_otsu_thresholding!\n");
	if(argc < 2){
		ERROR_LOG("no image path found in arguments\n");
		return -1;
	}

	Mat frame, bin_image;
	frame = imread(argv[1],CV_LOAD_IMAGE_COLOR);
	if(!frame.data){
		ERROR_LOG("Cannot open image\n");
		return -1;
	}
	cvtColor(frame,frame,CV_RGB2GRAY);
	float thresh;
	int error = preProcessing(frame,bin_image);
	if( error < 0){
		ERROR_LOG("error with otsuThresholding\n");
		return -1;
	}
	DEBUG_LOG("bin image size : (%d,%d)\n",bin_image.cols,bin_image.rows);

	namedWindow("original_image",WINDOW_NORMAL);
	imshow("original_image",frame);

	namedWindow("pre_processed_image",WINDOW_NORMAL);
	imshow("pre_processed_image",bin_image);

	waitKey(0);
	return 0;
}

int test_otsu_thresholding(int argc, char* argv[]){
	DEBUG_LOG("Inside test_otsu_thresholding!\n");
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
	float thresh;
	int error = otsuThresholding(frame,thresh);
	if( error < 0){
		ERROR_LOG("error with otsuThresholding\n");
	}
}

int test_thresholding(int argc, char* argv[]){
	DEBUG_LOG("Inside test_thresholding!");
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