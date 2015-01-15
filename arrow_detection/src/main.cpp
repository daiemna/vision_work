// main.cpp
#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>

#include "arrow_processing.hpp"
#include "debuging.hpp"

using namespace arrow_detection;
using namespace cv;
using namespace std;
using namespace cv_debuging;



int test_thresholding(int argc, char* argv[]);
int test_PreProcessing(int argc, char* argv[]);
int test_blurring(int argc, char* argv[]);
int test_findBlob(int argc,char* argv[]);
int extract_blobs(int argc,char* argv[]);
int test_feature_extraction(int argc,char* argv[]);
int extract_features(int argc,char* argv[]);
int arrow_recognition(int argc,char* argv[]);
void find_arrow(Mat frame, Mat left_means,Mat right_means, Mat non_means);
int classify_arrow(Mat left_means,Mat right_means,Mat non_means,Mat blob_features);
bool countNonZeroInCol(Mat frame,int from_col,int to_col,int thre);
bool hasHole(Mat arrow_rect);
int find_arrow_in_sceen(int argc,char* argv[]);

int main(int argc,char* argv[]){
	DEBUG_LOG("arrow_detection main!\n");
	// test_thresholding(argc,argv);
	// test_otsu_thresholding(argc,argv);
	// return test_PreProcessing(argc,argv);
	// return test_blurring(argc,argv);
	// test_findBlob(argc,argv);
	// extract_blobs(argc,argv);
	// test_feature_extraction(argc,argv);
	// extract_features(argc,argv);
	arrow_recognition(argc,argv);
	// find_arrow_in_sceen(argc,argv);
	int key;
	while((key = waitKey(0)) != 1048586){
		DEBUG_STREAM << "key was : "<<key <<endl;
	}
	return 0;
}

int find_arrow_in_sceen(int argc,char* argv[]){
	if(argc != 3){
		ERROR_LOG("please enter .csv file!\n and also the frame file");
		return -1;
	}
	ifstream in_file;
	in_file.open(argv[1]);
	if(!in_file.is_open()){
		ERROR_LOG("Unable to open .csv file");
		return -1;
	}
	string filepath;
	int label;
	float feat_1,feat_2,feat_3,feat_4;
	// double mean_1,mean_2,mean_3,mean_4;
	Mat left_arrow_means(1,4,CV_32F);
	Mat right_arrow_means(1,4,CV_32F);
	Mat non_arrow_means(1,4,CV_32F);
	float left_count =0;
	float right_count =0;
	float non_count =0;
	getline(in_file,filepath);
	while(!in_file.eof()){
		in_file >> filepath >> label >> feat_1 >> feat_2 >> feat_3 >> feat_4;
		// DEBUG_STREAM << filepath <<": "<< label << endl;
		if(label == 1){
			left_count++;
			left_arrow_means.at<float>(0) = left_arrow_means.at<float>(0) + feat_1;
			left_arrow_means.at<float>(1) = left_arrow_means.at<float>(1) + feat_2;
			left_arrow_means.at<float>(2) = left_arrow_means.at<float>(2) + feat_3;
			left_arrow_means.at<float>(3) = left_arrow_means.at<float>(3) + feat_4;
		}else if(label == 2){
			right_count++;
			right_arrow_means.at<float>(0) = right_arrow_means.at<float>(0) + feat_1;
			right_arrow_means.at<float>(1) = right_arrow_means.at<float>(1) + feat_2;
			right_arrow_means.at<float>(2) = right_arrow_means.at<float>(2) + feat_3;
			right_arrow_means.at<float>(3) = right_arrow_means.at<float>(3) + feat_4;
		}else{
			non_count++;
			non_arrow_means.at<float>(0) = non_arrow_means.at<float>(0) + feat_1;
			non_arrow_means.at<float>(1) = non_arrow_means.at<float>(1) + feat_2;
			non_arrow_means.at<float>(2) = non_arrow_means.at<float>(2) + feat_3;
			non_arrow_means.at<float>(3) = non_arrow_means.at<float>(3) + feat_4;
		}
	}
	left_arrow_means = left_arrow_means/left_count;

	right_arrow_means = right_arrow_means/right_count;

	non_arrow_means = non_arrow_means/non_count;

	DEBUG_LOG("left arrows : %f\n",left_count);
	DEBUG_LOG("right arrows : %f\n",right_count);
	DEBUG_LOG("non arrows : %f\n",non_count);

	// DEBUG_STREAM << "left average : " << left_arrow_means << endl;
	// DEBUG_STREAM << "Right average : " << right_arrow_means << endl;
	// DEBUG_STREAM << "non arrow average : " << non_arrow_means << endl;
	in_file.close();
 	DEBUG_LOG("Reading file : %s\n",argv[2]);
	Mat frame = imread(argv[2],CV_LOAD_IMAGE_COLOR);
	if(!frame.data){
		ERROR_LOG("ERROR reading file!\n");
		return -1;
	}
	resize(frame,frame,Size(1080,760));
	viewImage(frame,"Original Image");
    find_arrow(frame,left_arrow_means,right_arrow_means,non_arrow_means);
    // viewImage(frame,"processed image");
    return 0;

}

int arrow_recognition(int argc,char* argv[]){
	if(argc != 2){
		ERROR_LOG("please enter .csv file!\n");
		return -1;
	}
	ifstream in_file;
	in_file.open(argv[1]);
	if(!in_file.is_open()){
		ERROR_LOG("Unable to open .csv file");
		return -1;
	}
	string filepath;
	int label;
	float feat_1,feat_2,feat_3,feat_4;
	// double mean_1,mean_2,mean_3,mean_4;
	Mat left_arrow_means(1,4,CV_32F);
	Mat right_arrow_means(1,4,CV_32F);
	Mat non_arrow_means(1,4,CV_32F);
	float left_count =0;
	float right_count =0;
	float non_count =0;
	getline(in_file,filepath);
	while(!in_file.eof()){
		in_file >> filepath >> label >> feat_1 >> feat_2 >> feat_3 >> feat_4;
		// DEBUG_STREAM << filepath <<": "<< label << endl;
		if(label == 1){
			left_count++;
			left_arrow_means.at<float>(0) = left_arrow_means.at<float>(0) + feat_1;
			left_arrow_means.at<float>(1) = left_arrow_means.at<float>(1) + feat_2;
			left_arrow_means.at<float>(2) = left_arrow_means.at<float>(2) + feat_3;
			left_arrow_means.at<float>(3) = left_arrow_means.at<float>(3) + feat_4;
		}else if(label == 2){
			right_count++;
			right_arrow_means.at<float>(0) = right_arrow_means.at<float>(0) + feat_1;
			right_arrow_means.at<float>(1) = right_arrow_means.at<float>(1) + feat_2;
			right_arrow_means.at<float>(2) = right_arrow_means.at<float>(2) + feat_3;
			right_arrow_means.at<float>(3) = right_arrow_means.at<float>(3) + feat_4;
		}else{
			non_count++;
			non_arrow_means.at<float>(0) = non_arrow_means.at<float>(0) + feat_1;
			non_arrow_means.at<float>(1) = non_arrow_means.at<float>(1) + feat_2;
			non_arrow_means.at<float>(2) = non_arrow_means.at<float>(2) + feat_3;
			non_arrow_means.at<float>(3) = non_arrow_means.at<float>(3) + feat_4;
		}
	}
	left_arrow_means = left_arrow_means/left_count;

	right_arrow_means = right_arrow_means/right_count;

	non_arrow_means = non_arrow_means/non_count;

	DEBUG_LOG("left arrows : %f\n",left_count);
	DEBUG_LOG("right arrows : %f\n",right_count);
	DEBUG_LOG("non arrows : %f\n",non_count);

	DEBUG_STREAM << "left average : " << left_arrow_means << endl;
	DEBUG_STREAM << "Right average : " << right_arrow_means << endl;
	DEBUG_STREAM << "non arrow average : " << non_arrow_means << endl;
	in_file.close();

	VideoCapture cap(0); // open the default camera
    if(!cap.isOpened()){  // check if we succeeded
    	ERROR_LOG("Unable to open Video Cam");
    	return -1;
    }
    Mat edges;
    // namedWindow("live feed",1);
    for(;;)
    {
    	Mat frame;
        cap >> frame; // get a new frame from camera
        find_arrow(frame,left_arrow_means,right_arrow_means,non_arrow_means);
        viewImage(frame,"live feed");
        if(waitKey(10) == 1048586) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;

}

void find_arrow(Mat frame, Mat left_means,Mat right_means, Mat non_means){
	Mat binary,gray_frame;
	vector < std::vector<cv::Point2i > > blobs;
	vector<Rect> blobs_roi;
	if(frame.channels() == 3){
		// DEBUG_LOG("coloured image: changing to gray!\n");
		cvtColor(frame,gray_frame,CV_RGB2GRAY);
	}

	preProcessing(gray_frame, binary);
	viewImage(binary*255,"Binary Image");
	segmentation(binary,blobs_roi, blobs);
	viewSegmentedImage(binary,blobs);
	for(int i = 0; i < blobs_roi.size();i++){
		Mat blob(gray_frame,blobs_roi[i]),blob_feat(1,4,CV_32F);
		vector<float> blob_features;
		if(featureExteraction(blob,blob_features) < 0){
			continue;
		}
		blob_feat.at<float>(0) = blob_features[0];
		blob_feat.at<float>(1) = blob_features[1];
		blob_feat.at<float>(2) = blob_features[2];
		blob_feat.at<float>(3) = blob_features[3];
		int blob_label = classify_arrow(left_means,right_means,non_means,blob_feat);
		if(blob_label == 2 || blob_label == 1){
			Mat bin_blob(binary,blobs_roi[i]);
			if(hasHole(bin_blob)){
				continue;
			}
			// DEBUG_LOG("________different blob___________\n");
			// DEBUG_STREAM << "BLOBS rect " << blobs_roi[i] << endl;
			if(countNonZeroInCol(bin_blob,0,3,11)){
				// DEBUG_LOG("its Left!\n");
				rectangle(frame,blobs_roi[i],Scalar(0,255,0));
				putText(frame, "Left", Point(blobs_roi[i].x,blobs_roi[i].y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0));
			}else if(countNonZeroInCol(bin_blob,bin_blob.cols-4,bin_blob.cols-1,11)){
				// DEBUG_LOG("its Right!\n");
				rectangle(frame,blobs_roi[i],Scalar(0,0,255));
				putText(frame, "Right", Point(blobs_roi[i].x,blobs_roi[i].y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255));
			}
		}
	}
	viewImage(frame,"Arrow Found");
}
bool countNonZeroInCol(Mat frame,int init,int end,int threshold){
	// int init = from_col > to_col ? to_col : from_col;
	// int end = from_col < to_col ? to_col : from_col;
	for(int i = init; i <= end; i++){
		// DEBUG_LOG("col #%d countNonZero : %d\n",i,countNonZero(frame.col(i)));
		if(countNonZero(frame.col(i)) > threshold){
			return false;
		}
	}
	return true;
}
bool hasHole(Mat arrow_rect){
	for(int ind=0; ind < arrow_rect.cols; ind++){
		Mat column = arrow_rect.col(ind);
		int pre = 0;
		bool started=false,ended = false;
		for(int i = 0; i < column.rows; i++){
			if(pre == 0 && column.at<int>(i) == 1){
				started = true;
			}
			if(started && pre == 1 && column.at<int>(i) == 0){
				ended = true;
			}
			if(ended && pre == 0 && column.at<int>(i) == 1){
				return true;
			}
			pre = column.at<int>(i);
		}
	}
	return false;
}
int classify_arrow(Mat left_means,Mat right_means,Mat non_means,Mat blob_features){
	double dist_left = norm(left_means,blob_features,NORM_INF);
	double dist_right = norm(right_means,blob_features,NORM_INF);
	double dist_non = norm(non_means,blob_features,NORM_INF);
	if(dist_left < dist_right && dist_left < dist_non){
		return 1;
	}else if(dist_right < dist_left && dist_right < dist_non){
		return 2;
	}else{
		return 0;
	}

}
int extract_features(int argc,char* argv[]){
	if(argc != 3){
		ERROR_LOG("no input folder name given or no out file name given");
		return -1;
	}
	string dir(argv[1]), delm = " ";
	DIR *dp,*odp;
	struct dirent *dirp;
	struct stat filestat;
	ofstream out_file;

	dp = opendir(argv[1]);
	// odp = opendir(argv[2]);
	if (dp == NULL || odp == NULL){
		ERROR_LOG("Error(%d): opening %s\n",errno,argv[1]);
		return errno;
	}
	out_file.open(argv[2]);
	if(!out_file.is_open()){
		ERROR_LOG("Cannot open file %s\n",argv[2]);
		return -1;
	}
	out_file << "file_path" <<delm<<"label"<<delm<<"feature_1"<<delm<<"feature_2"<<delm<<"feature_3"<<delm<<"feature_4"<<delm << endl;
	while((dirp = readdir( dp ))){
		string filepath = dir + "/" + dirp->d_name;

		// If the file is a directory (or is in some way invalid) we'll skip it 
		if (stat(filepath.c_str(), &filestat)) continue;
		if (S_ISDIR(filestat.st_mode))         continue;

		DEBUG_LOG("reading file %s\n",filepath.c_str());
		Mat frame;
		frame = imread(filepath.c_str(),CV_LOAD_IMAGE_ANYDEPTH);
		if(!frame.data){
			ERROR_LOG("Cannot open image %s\n",filepath.c_str());
			continue;
		}
		if(frame.channels() == 3){
			// DEBUG_LOG("coloured image: changing to gray!\n");
			cvtColor(frame,frame,CV_RGB2GRAY);
		}
		vector<float> features;
		if(featureExteraction(frame,features) < 0){
			ERROR_LOG("Error in feature Exteraction\n");
			continue;
		}
		// DEBUG_LOG("found Left at: %d\n",filepath.find("Left"));
		// DEBUG_LOG("found left at: %d\n",filepath.find("left"));
		// DEBUG_LOG("found none at: %d\n",filepath.find("None"));
		DEBUG_STREAM << "value returned by find :" << filepath.find("Right") << endl;

		if(filepath.find("Left")!= std::string::npos || filepath.find("left")!= std::string::npos){
			DEBUG_LOG("Left arrow!\n");
			out_file <<filepath << delm<<"1"<<delm<< features[0]<< delm<<features[1]<< delm<<features[2]<< delm<<features[3]<< delm << endl;
		}else if(filepath.find("Right")!= std::string::npos || filepath.find("right")!= std::string::npos){
			DEBUG_LOG("Right arrow!\n");
			out_file <<filepath << delm<<"2"<<delm<< features[0]<< delm<<features[1]<< delm<<features[2]<< delm<<features[3]<< delm << endl;
		}else{
			DEBUG_LOG("Not an arrow!\n");
			out_file <<filepath << delm<<"0"<<delm<< features[0]<< delm<<features[1]<< delm<<features[2]<< delm<<features[3]<< delm << endl;
		}
	}
	closedir(dp);
	out_file.close();
	return 0;
}

int test_feature_extraction(int argc,char* argv[]){
	if(argc < 2){
		ERROR_LOG("no image path found in arguments\n");
		return -1;
	}

	Mat frame;
	frame = imread(argv[1],CV_LOAD_IMAGE_ANYDEPTH);
	if(!frame.data){
		ERROR_LOG("Cannot open image\n");
		exit(-1);
	}
	if(frame.channels() == 3){
		DEBUG_LOG("coloured image: changing to gray!\n");
		cvtColor(frame,frame,CV_RGB2GRAY);
	}

	vector<float> features;
	featureExteraction(frame,features);
	DEBUG_STREAM << "features are : " << features[0] << "," << features[1] << endl;

	return 0;
}

int extract_blobs(int argc,char* argv[]){
	if(argc != 3){
		ERROR_LOG("enter input adn output folder name!\n");
		return -1;
	}
	string dir(argv[1]),outdir(argv[2]),filepath,op_filename;
	DIR *dp,*odp;
	struct dirent *dirp;
	struct stat filestat;
	

	dp = opendir(argv[1]);
	// odp = opendir(argv[2]);
	if (dp == NULL || odp == NULL){
		ERROR_LOG("Error(%d): opening %s\n",errno,argv[1]);
		return errno;
	}
	int frame_num = 139;
	while((dirp = readdir( dp ))){
		filepath = dir + "/" + dirp->d_name;

		// If the file is a directory (or is in some way invalid) we'll skip it 
		if (stat(filepath.c_str(), &filestat)) continue;
		if (S_ISDIR(filestat.st_mode))         continue;

		DEBUG_LOG("reading file %s\n",filepath.c_str());
		Mat frame,binary,color_frame;
		vector < std::vector<cv::Point2i > > blobs;
		vector<Rect> blobs_roi;

		color_frame = imread(filepath.c_str(),CV_LOAD_IMAGE_COLOR);
		frame = imread(filepath.c_str(),CV_LOAD_IMAGE_GRAYSCALE);
		preProcessing(frame, binary);
		segmentation(binary,blobs_roi, blobs);
		int num = 1;
		for(Rect blob_roi : blobs_roi){
    		// Size sz(blob_roi.width,blob_roi.height);
    		// Point ofs(blob_roi.x,blob_roi.y);
			Mat sub_image(color_frame,blob_roi);
    		// ostringstream convert;
    		// convert << num;
			op_filename = outdir+"/Clip_"+ to_string(frame_num)+to_string(num) +".jpg";
			imwrite(op_filename.c_str(),sub_image);
			num++;
		}
		frame_num++;
	}
	closedir( dp );
}
int test_findBlob(int argc,char* argv[]){
	if(argc < 2){
		ERROR_LOG("no image path found in arguments\n");
		return -1;
	}

	Mat frame;
	frame = imread(argv[1],CV_LOAD_IMAGE_ANYDEPTH);
	if(!frame.data){
		ERROR_LOG("Cannot open image\n");
		exit(1);
	}
	if(frame.channels() == 3){
		DEBUG_LOG("coloured image: changing to gray!\n");
		cvtColor(frame,frame,CV_RGB2GRAY);
	}
	DEBUG_STREAM << "frame type: " << frame.type() << endl;

	DEBUG_LOG("frame min, max : ");
	printMinMax(frame);

	namedWindow("binary",WINDOW_NORMAL);
	namedWindow("labelled",WINDOW_NORMAL);

	Mat output = Mat::zeros(frame.size(), CV_8UC3);

	Mat binary;
	vector < std::vector<cv::Point2i > > blobs;
	vector<Rect> blobs_roi;

	preProcessing(frame, binary);
	segmentation(binary,blobs_roi, blobs);
    // Randomy color the blobs
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
		rectangle(frame, blobs_roi[i],Scalar(255,0,0));
	}
	viewImage(frame,"original iamge");
	imshow("binary", binary*255);
	imshow("labelled", output);
    // waitKey(0);

	return 0;
}
int test_blurring(int argc, char* argv[]){
	DEBUG_LOG("Inside bluring\n!");
	if(argc < 2){
		ERROR_LOG("no image path found in arguments\n");
		return -1;
	}

	Mat frame, bin_image;
	frame = imread(argv[1],0);
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
	DEBUG_LOG("Inside test_PreProcessing!\n");
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
	if(frame.channels() >= 3){
		DEBUG_LOG("converting to gray\n");
		cvtColor(frame,frame,CV_RGB2GRAY	);
	}
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

	// waitKey(0);
	return 0;
}
int test_thresholding(int argc, char* argv[]){
	DEBUG_LOG("Inside test_thresholding!");
	if(argc < 2){
		ERROR_LOG("no image path found in arguments\n");
		return -1;
	}

	Mat frame, bin_image;
	frame = imread(argv[1],0);
	if(!frame.data){
		ERROR_LOG("Cannot open image\n");
	}
	if(frame.channels() >= 3){
		cvtColor(frame,frame,CV_RGB2GRAY);
	}
	DEBUG_STREAM << "frame type: " << frame.type() << endl;

	namedWindow( "RESULT", WINDOW_NORMAL );
	int key = 0;
	double thresh = 0;
	while((key = waitKey(0)) != 1048586){
		DEBUG_STREAM << "key was : " << key << endl;
		threshold( frame, bin_image, 0, thresh,THRESH_BINARY_INV);
		DEBUG_LOG("min max binary image: ");
		printMinMax(bin_image);
		if(bin_image.data)
			imshow( "RESULT", bin_image);
		if(key == 1114027){
			thresh++;
			if(thresh > 255)
				thresh = 255;
		}else if(key == 1114029){
			thresh--;
			if (thresh < 0)
				thresh = 0;
		}
		DEBUG_STREAM << "Threshhold is: " << thresh << endl;
	}
	return 0;
}

