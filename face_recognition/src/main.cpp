// main.ccp 
// creator: Daiem Ali
// Date: 23/10/14



#include <opencv2/opencv.hpp>
#include <stdio.h>

#include "image_stat/image_stat.hpp"
#include "face_recognition.hpp"

using namespace cv;
using namespace image_stat;
using namespace face_recognition;
using namespace std;

int main(int argc, char** argv) {
	Mat image_1,image_2;
	image_1 = imread(argv[1], 1);
	image_2 = imread(argv[2], 1);

	Size size(100,100);//the dst image size,e.g.100x100
	resize(image_1,image_1,size);//resize image
	resize(image_2,image_2,size);//resize image

	printf("image 1 size (%d,%d)\n",image_1.rows,image_1.cols);
	printf("image 2 size (%d,%d)\n",image_2.rows,image_2.cols);
	int code;
	float MI = 0.0;

	vector<Mat> imageList;
	// imageList.resize(2);
	imageList.push_back(image_2);
	imageList.push_back(image_1);

	vector<int> sortedIndex;
	if((code = similarityOrderedList(image_1,&imageList,&sortedIndex,&mutual_information)) != 0){
		printf("ERROR in  similarityOrderedList %d \n",code);
	}
	printf("MI: %lf \n", MI);
	waitKey(0);

	return 0;
}
