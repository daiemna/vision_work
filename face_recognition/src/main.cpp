// main.ccp 
// creator: Daiem Ali
// Date: 23/10/14



#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
// #include <cvplot.h>

#include "image_stat/image_stat.hpp"
#include "face_recognition.hpp"
#include "debuging.h"

#define PRESISSION float

using namespace cv;
using namespace image_stat;
using namespace face_recognition;
using namespace std;
using namespace std_conversion;
// using namespace CvPlot;

int test(int argc, char** argv);
int make_image_pair(int,char**);
int evaluate_pairs(int argc,char** argv);
int test_matchTemplate(int,char**);

int genrate_roc_data(const PRESISSION  thresh,size_t pos_count,size_t neg_count,Mat& r_val_list,Mat& label_list,PRESISSION& TPR, PRESISSION& FPR, PRESISSION& ACC);
int plot2D(Mat &x,Mat &y,Mat& z);

// PlotManager pm;

#define SIM_MAT SSIM

ofstream log_file;

// #define cout log_file

int main(int argc, char** argv) {
	log_file.open("log.txt");

	
	if(DEBUG)
		cout << "in main" << endl;
	if(DEBUG)
		cout << "int bytes : " << sizeof(unsigned int)<< endl;

	make_image_pair(argc, argv);
	evaluate_pairs(argc,argv);
	// test(argc,argv);
	
	// pm();

	// test_matchTemplate(argc,argv);
	log_file.close();
	waitKey(0);
}

int evaluate_pairs(int argc,char** argv){
	int max_pairs = 100;

	if(DEBUG)
		cout << "in evaluate_pairs" << endl;
	if(DEBUG)
		cout << "argc : "<< argc << endl;

	if(argc == 3){
		max_pairs = atoi(argv[2]);
	}

	ifstream fin;
	Mat r_val_list(max_pairs,1,CV_32F);
	Mat label_list_DC(max_pairs,1,CV_32F);
	Mat label_list(max_pairs,1,CV_32SC1);
	
	if(DEBUG)
		cout << "file: " << argv[1] << endl;

	string file_1,file_2,label,r_val;
	fin.open(strcat(argv[1],".txt"));
	PRESISSION relevance;
	unsigned int pair_label;
	double min_val,max_val;
	
	for(int i = 0;i < max_pairs; i++){
		fin >> file_1 >> file_2 >> label >> r_val;
		relevance = stof(r_val);
		label_list_DC.at<PRESISSION>(i,0) = stof(label);
		istringstream reader(label);
		reader >> pair_label;
		label_list.at<unsigned int>(i,0) = pair_label;
		r_val_list.at<PRESISSION>(i,0) = relevance;
		// if(DEBUG)
		// 	cout << file_1<< " " << file_2<< " " << pair_label<< " " << relevance << endl;
	}
	Mat one_sum;
	size_t negtivePairs_count,positivePairs_count;
	cv::minMaxLoc(r_val_list, &min_val, &max_val);
	reduce(label_list_DC,one_sum,0,CV_REDUCE_SUM);
	cout << "sum after reduction : " << one_sum.at<PRESISSION>(0,0) << endl;
	positivePairs_count = one_sum.at<PRESISSION>(0,0);
	negtivePairs_count = label_list.total() - positivePairs_count;

	// if(DEBUG){
		cout << "positivePairs_count : " << positivePairs_count << endl;
		cout << "negtivePairs_count : " << negtivePairs_count << endl;
		cout << "r_val_list : " << label_list.size().area() << endl;
		cout << "min , max : " << (float)min_val << ", " << (float)max_val << endl;
	// }
	  
	// Mat thresh_list;
	PRESISSION tpr,fpr,acc;
	tpr = fpr = acc = 0.0;

	int ind = 0;
	PRESISSION thresh_jump = 10;
	/*****************************************************/
	/*CAREFULLY UNDERSTAND the dimmentions of each matrix*/
	/*****************************************************/
	size_t sample_count = cvRound((max_val - min_val )/ thresh_jump);
	 // size_t sample_count = r_val_list.total();
	Mat tpr_list(sample_count,1,CV_32F),
	fpr_list(sample_count,1,CV_32F),
	acc_list(sample_count,1,CV_32F);

	for(PRESISSION thresh = (PRESISSION)min_val; thresh <= (PRESISSION)max_val ; thresh += thresh_jump, ind++){
		// cout << "thresh : " << thresh << endl;
		genrate_roc_data(thresh,positivePairs_count,negtivePairs_count,r_val_list,label_list,tpr,fpr,acc);
		// log_file << "tpr : " << tpr <<  endl;
		// log_file << "fpr : " << fpr <<  endl;
		// log_file << "acc : " << acc <<  endl;
		tpr_list.at<PRESISSION>(ind,0) = tpr;
		fpr_list.at<PRESISSION>(ind,0) = fpr;
		acc_list.at<PRESISSION>(ind,0) = acc;
		// cout << "acc is : "<< acc << endl;
	}
	// log_file << "tpr_list : " << tpr_list << endl;
	// log_file << "fpr_list : " << fpr_list << endl;
	// transpose(acc_list,acc_list);
	// log_file << "acc_list : " << acc_list << endl;
	plot2D(tpr_list,fpr_list,acc_list);

	fin.close();
	return 0;
}

int plot2D(Mat &x,Mat &y,Mat& z){
	ofstream outfile;
	outfile.open("../aux/result.txt",ofstream::out | ofstream::trunc);
	for(int i = 0 ; i < x.total(); i++){
		outfile << x.at<PRESISSION>(i,0) << " " << y.at<PRESISSION>(i,0) << " " << z.at<PRESISSION>(i,0) << endl;
	}
}

int genrate_roc_data(const PRESISSION thresh,size_t pos_count,size_t neg_count,Mat& r_val_list,Mat& label_list,PRESISSION& TPR, PRESISSION& FPR, PRESISSION& ACC){
	// cout << "type r_val_list: "<< r_val_list << endl;
	// cout << "type pos_count, neg_count "<< pos_count << ", " << neg_count << endl;
	// cout << "labels : " << label_list;
	// threshold(r_val_list,thresh_list,thresh,1,THRESH_BINARY);
	PRESISSION TP,TN,FP,FN;
	TP = TN = FP = FN = 0.0;
	PRESISSION class_val = 0;
	for(int i = 0; i < r_val_list.total() ; i++){
		if(r_val_list.at<PRESISSION>(i,0) <= thresh){
			class_val = 0;
		}else{
			class_val = 1;
		}
		// cout << "class_val : "<< class_val << endl;
		if(class_val == 1 && label_list.at<unsigned int>(i,0) == 1)
			TP++;
		else if(class_val == 1 && label_list.at<unsigned int>(i,0) == 0)
			FP++;
		else if(class_val == 0 && label_list.at<unsigned int>(i,0) == 1)
			FN++;
		else if(class_val == 0 && label_list.at<unsigned int>(i,0) == 0)
			TN++;
	}
	TPR = TP/pos_count;
	FPR = FP/neg_count;
	ACC = (TP+TN)/(pos_count + neg_count);
}

int make_image_pair(int argc, char* argv[]){
	// attain two dir pointer
	// read one file and start from next file and 
	// make combinations for each file.

	string filepath, filepath2;
	string file1_name,file2_name;
	long positivePairs = 0;
	long negtivePairs = 0;
	DIR *dp, *dp2;
	struct dirent *dirp, *dirp2;
	struct stat filestat;
	Mat image_1,image_2,img;
	vector<Mat> images;
	Size size(100,100);
	float r_val;
	string dir(argv[1]);

	// cout << "dir to get files of: " << flush;
	// getline( cin, dir );  // gets everything the user ENTERs

	dp = opendir( dir.c_str() );
	if (dp == NULL){
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}
	// namedWindow(dir.c_str(), CV_WINDOW_AUTOSIZE );
	ofstream outfile;
	outfile.open(dir+".txt",ofstream::out | ofstream::trunc);

	int max_lib = 100;
	// if(argc == 3)
	// 	max_lib = atoi(argv[2]);
	int lib_count= 0;

	//making an image library:
	while (lib_count <= max_lib){
		if(!(dirp = readdir(dp))){
			cerr << "max_lib : " << max_lib << endl;
			cerr << "Unable to make image lib " << endl;
			// exit(0);
			// break;
		}
		filepath = dir + "/" + dirp->d_name;
		// If the file is a directory (or is in some way invalid) we'll skip it 
		if(stat( filepath.c_str(), &filestat )){
			cout << "invalid file!" << filepath << endl ;
			continue;
		}
		if(S_ISDIR( filestat.st_mode )){
			cout << "found directory!" << filepath << endl ;
			continue;
		}
		img = imread(filepath.c_str(), 1);
		resize(img, img, size);
		images.push_back(img);
		lib_count++;
	}

	int code;

	while ((dirp = readdir(dp))){
		filepath = dir + "/" + dirp->d_name;
		file1_name = dirp->d_name;
		// If the file is a directory (or is in some way invalid) we'll skip it 
		if(stat( filepath.c_str(), &filestat )){
			cout << "invalid file!" << filepath << endl ;
			continue;
		}
		if(S_ISDIR( filestat.st_mode )){
			cout << "found directory!" << filepath << endl ;
			continue;
		}

		dp2 = opendir( dir.c_str() );
		if (dp2 == NULL){
			cout << "Error(" << errno << ") opening second time" << dir << endl;
			return errno;
		}
		while((dirp2 = readdir(dp2))) {

			filepath2 = dir + "/" + dirp2->d_name;
			file2_name = dirp2->d_name;
		    // If the file is a directory (or is in some way invalid) we'll skip it 
			if(stat( filepath2.c_str(), &filestat )){
				cout << "invalid file!" << filepath2 << endl ;
				continue;
			}
			if(S_ISDIR( filestat.st_mode )){
				cout << "found directory!" << filepath2 << endl ;
				continue;
			}

			if(file1_name.compare(file2_name) == 0){
				// cout << "same file!" << endl;
				continue;				
			}else if(file1_name.substr(0,6).compare(file2_name.substr(0,6)) == 0){
				image_1 = imread(filepath.c_str(), 1);
				resize(image_1, image_1, size);

				image_2 = imread(filepath2.c_str(), 1);
				resize(image_2, image_2, size);

				if((code = relevanceMeasure(image_1,image_2,&images, lib_count,&r_val,&SIM_MAT)) != 0){
					printf("ERROR in  relevanceMeasure %d \n",code);
					continue;
				}

				outfile << file1_name << " " << file2_name << " 1 "<< r_val << endl;
				positivePairs++;
			}else if(positivePairs > negtivePairs){
				image_1 = imread(filepath.c_str(), 1);
				resize(image_1, image_1, size);

				image_2 = imread(filepath2.c_str(), 1);
				resize(image_2, image_2, size);

				if((code = relevanceMeasure(image_1,image_2,&images, lib_count,&r_val,&SIM_MAT)) != 0){
					printf("ERROR in  relevanceMeasure %d \n",code);
					continue;
				}
				outfile << file1_name << " " << file2_name << " 0 "<< r_val << endl;
				negtivePairs++;
			}
			
		}
		closedir(dp2);
		// cout << "found file : " << filepath << endl ;
		// img = imread(filepath);
		// printf("image size (%d,%d)\n",img.rows,img.cols);
		// imshow(dir.c_str(),img);

		// waitKey(0);
	}
	cout << "positivePairs count : " << positivePairs << endl;
	cout << "negtivePairs count : " << negtivePairs << endl;

	closedir( dp );
	outfile.close();
	cout  << "key was : "<< waitKey(0) << endl;
	return 0;
}



int test(int argc, char** argv){
	Mat image_1,image_2,img;
	vector<Mat> images;
	Size size(100,100);//the dst image size,e.g.100x100

	image_1 = imread(argv[1], 1);
	resize(image_1, image_1, size);

	image_2 = imread(argv[2], 1);
	resize(image_2, image_2, size);


	for(int  i = 3; i < argc; i++){
		img = imread(argv[i], 1);
		resize(img, img, size);	
		images.push_back(img);
		printf("img size (%d,%d)\n",img.rows,img.cols);
	}

	int code;
	float relevance = 0.0;

	Mat sortedIndex;
	Mat uList;

	if((code = relevanceMeasure(image_1,image_2,&images, 100,&relevance,&SIM_MAT)) != 0){
		printf("ERROR in  relevanceMeasure %d \n",code);
	}
	cout << "relevance : " << relevance << endl;
	waitKey(0);

	return 0;
}

int test_matchTemplate(int argc,char** argv){
	
	if(argc < 3){
		cerr << "Error: need two image paths";
		return -1;
	}

	Mat image_1,image_2,img;
	Size size(100,100);//the dst image size,e.g.100x100

	image_1 = imread(argv[1], 1);
	resize(image_1, image_1, size);

	image_2 = imread(argv[2], 1);
	resize(image_2, image_2, size);

	Mat op;
	float op_float;
	SIM_MAT(image_1,image_2,&op_float);

	cout << "output : " << op_float << endl;

	return 0;
}