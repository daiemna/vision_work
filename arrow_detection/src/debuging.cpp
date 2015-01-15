// debuging.cpp
#include "debuging.hpp"

int cv_debuging::viewImage(Mat frame,string name){
	namedWindow( name, WINDOW_NORMAL );
	imshow(name,frame);
	if(WRITE_VIEWED_IMG){
		imwrite(name + ".jpg",frame);
	}
	return 0;
}

int cv_debuging::printMinMax(Mat image){
	double minVal=0,maxVal=0;
    // binary.convertTo(image, CV_32SC1);
    minMaxLoc(image,&minVal,&maxVal);
    DEBUG_LOG("[min,max] : [%lf,%lf]\n",minVal,maxVal);
}

int cv_debuging::viewSegmentedImage(Mat frame,vector < std::vector<cv::Point2i > > &blobs,string name){
	// Randomly color the blobs
	Mat output = Mat::zeros(frame.size(), CV_8UC3);
	for(size_t i=0; i < blobs.size(); i++) {
		unsigned char r = 255 * (rand()/(1.0 + RAND_MAX));
		unsigned char g = 255 * (rand()/(1.0 + RAND_MAX));
		unsigned char b = 255 * (rand()/(1.0 + RAND_MAX));

		for(size_t j=0; j < blobs[i].size(); j++) {
			int x = blobs[i][j].x;
			int y = blobs[i][j].y;

			output.at<Vec3b>(y,x)[0] = b;
			output.at<Vec3b>(y,x)[1] = g;
			output.at<Vec3b>(y,x)[2] = r;
		}
		// rectangle(frame, blobs_roi[i],Scalar(255,0,0));
	}
	viewImage(output,name);
	return 0;
}