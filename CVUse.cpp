#include "pch.h"
#include <iostream>
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;

			 

int main(int argc, char **argv){
				 


	Mat src; Mat srcGray; Mat srcThresh; Mat cannyOut;
	RNG rng(12345);
	double otsu;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	string sObjectNumber;


		 src = imread("car_on_the_road2.jpg", 1);

				 
		 cvtColor(src, srcGray, COLOR_BGR2GRAY);
		 blur(srcGray, srcGray, Size(3, 3));

		otsu = threshold(srcGray, srcThresh, 0, 255, THRESH_BINARY | THRESH_OTSU);

				
		Canny(srcGray, cannyOut, otsu, otsu * 1 / 2, 3, 1);


		 findContours(cannyOut, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

				 
		 vector<Moments> mu(contours.size());
		 for (int i = 0; i < contours.size(); i++){
			
			 mu[i] = moments(contours[i], false);
			
		 }

				 
		 vector<Point2f> mc(contours.size());
		
			 for (int i = 0; i < contours.size(); i++){
				 
				 mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
			}

				 
		Mat drawing = Mat::zeros(cannyOut.size(), CV_8UC3);
		
		         
				 ostringstream sContourNumber;   

				 for (int i = 0; i < contours.size(); i++) {

					 sContourNumber << "(" << round(mc[i].x) << ";" << round(mc[i].y) << ")";
					 sObjectNumber = sContourNumber.str();

					 Point pCoordinates(mc[i].x + 5, mc[i].y - 7);
					 Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

					 drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
					 drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());

					 circle(src, mc[i], 4, color, -1, 8, 0);
					 circle(drawing, mc[i], 4, color, -1, 8, 0);

					 putText(src, sObjectNumber, pCoordinates, FONT_HERSHEY_COMPLEX, 1, color, 2, 8);
					 putText(drawing, sObjectNumber, pCoordinates, FONT_HERSHEY_COMPLEX, 1, color, 2, 8);

					 sContourNumber.str("");
					 sContourNumber.clear();


				 }

				 resize(src, src, src.size() / 2);
				 resize(drawing, drawing, drawing.size() / 2);


				 imshow("OriginalWithContours",src);
				 imshow("Contours", drawing);

				 imwrite("result.jpg", drawing);

				 waitKey(0);
				 
				 return(0);

}