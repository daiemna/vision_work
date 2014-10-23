// main.ccp 
// creator: Daiem Ali
// Date: 23/10/14



#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

int main(int argc, char** argv) {
	Mat image;
	image = imread(argv[1], 1);

	if (argc != 2 || !image.data) {
		printf("No image data \n");
		return -1;
	}
	printf("Image read ok \n");
	printf("size win: %d,%d \n",image.rows,image.cols);
	namedWindow("Display Image", WINDOW_NORMAL);
	imshow("Display Image", image);

	waitKey(0);

	return 0;
}
