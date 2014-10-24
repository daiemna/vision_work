// image_stat.hpp
// Daiem Ali
// 23/10/14

#ifndef IMAGE_STAT_HPP
#define IMAGE_STAT_HPP

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
namespace image_stat {

	#define ERROR_MI_IMAGE_DATA -44 
	#define ERROR_MI_IMAGE_MISALIGNED -14

	int mutual_information(Mat image_1,Mat image_2,float *mi){
		cout << "Using MI"<< endl;
		if( image_1.rows == 0  || image_1.cols == 0){ 
			return ERROR_MI_IMAGE_DATA;
		}
		if(image_1.rows != image_2.rows || image_1.cols != image_2.cols){
			return ERROR_MI_IMAGE_MISALIGNED;
		}
		Mat gray_1,gray_2;
		cvtColor(image_1, gray_1, CV_RGB2GRAY);
		cvtColor(image_2, gray_2, CV_RGB2GRAY);

		/// calcHis Params
		int histSize = 256;
		float range[] = { 0, 255 } ;
		const float* histRange = { range };
		bool uniform = true; 
		bool accumulate = false;

		Mat hist_1,hist_2;

		/// Compute the histograms:
		calcHist( &gray_1, 1, 0, Mat(), hist_1, 1, &histSize, &histRange, uniform, accumulate );
		calcHist( &gray_2, 1, 0, Mat(), hist_2, 1, &histSize, &histRange, uniform, accumulate );

		// converting to row vector
		hist_1.reshape(0,1);
		hist_2.reshape(0,1);

		Mat pdf_1(hist_1.rows,hist_1.cols,CV_32F),
		pdf_2(hist_1.rows,hist_1.cols,CV_32F);

		long size_1 = image_1.rows * image_1.cols,
		size_2 = image_2.rows * image_2.cols;

		// Probiblit Density Function(PDF)
		pdf_1 = hist_1/size_1;
		pdf_2 = hist_2/size_2;

		//Joint PDF
		Mat jpdf(hist_1.rows,hist_1.cols,CV_32F);
		jpdf = (hist_1+hist_2)/(size_1+size_2);

		// replace 0 with 1 in all 3 PDF
		// cause as x -> 0 ; log(x) -> -Inf
		Mat thr_pdf_1(hist_1.rows,hist_1.cols,CV_32F),
		thr_pdf_2(hist_1.rows,hist_1.cols,CV_32F),
		thr_jpdf(hist_1.rows,hist_1.cols,CV_32F);

		threshold(pdf_1,thr_pdf_1,0,1,THRESH_BINARY_INV);
		threshold(pdf_2,thr_pdf_2,0,1,THRESH_BINARY_INV);
		threshold(jpdf,thr_jpdf,0,1,THRESH_BINARY_INV);

		thr_pdf_1 = thr_pdf_1 + pdf_1;
		thr_pdf_2 = thr_pdf_2 + pdf_2;
		thr_jpdf = thr_jpdf + jpdf;


		// calculating Entropy
		float E1 = 0.0,E2 = 0.0,E12 = 0.0;
		Mat log_pdf_1,log_pdf_2,log_jpdf;
		Mat E1_mat(1,1,CV_32F),
		E2_mat(1,1,CV_32F),
		E12_mat(1,1,CV_32F);


		cv::log(thr_pdf_1,log_pdf_1);
		reduce(thr_pdf_1.mul(log_pdf_1),E1_mat,0,CV_REDUCE_SUM);
		E1 = -1 * E1_mat.at<float>(0);

		cv::log(thr_pdf_2,log_pdf_2);
		reduce(thr_pdf_2.mul(log_pdf_2),E2_mat,0,CV_REDUCE_SUM);
		E2 = -1 * E2_mat.at<float>(0,0);

		cv::log(thr_jpdf,log_jpdf);
		reduce(thr_jpdf.mul(log_jpdf),E12_mat,0,CV_REDUCE_SUM);
		E12 = -1 * E12_mat.at<float>(0,0);

		*mi =  E1 + E2 - E12;

		return 0;
	}
} // image_stat


#endif // IMAGE_STAT_HPP
