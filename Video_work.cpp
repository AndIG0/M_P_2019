﻿// Video_work.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



#define VIDEO_PATH "sd.mp4"

int main(){
	
	
	VideoCapture videoSource;
	Mat frame; Mat srcGray; Mat Traectory;Mat srcThresh;Mat cannyOut;
	double otsu;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	string sObjectNumber;
	
	
	if (!videoSource.open(VIDEO_PATH))
	{
		std::cout << "Video not found at " << VIDEO_PATH << std::endl;
		return 1;     
	}
	videoSource.set(CAP_PROP_CONVERT_RGB, 1);


	
	while (true)
	{
		videoSource >> frame;

		

		Rect r(0,frame.rows*(0.1),frame.cols, frame.rows*(0.9));
		frame(r).copyTo(frame);


		if (frame.empty())
			break;
		
		cvtColor(frame, srcGray, COLOR_BGR2GRAY);
		blur(srcGray, srcGray, Size(3, 3));

		otsu = threshold(srcGray, srcThresh, 0, 255, THRESH_BINARY | THRESH_OTSU);

		
		
		Canny(srcGray, cannyOut, otsu, otsu * 1 / 2, 3, 1);


		
		findContours(cannyOut, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));


		vector<Moments> mu(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i], false);
		}


		vector<Point2f> mc(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}


		ostringstream sContourNumber;

		for (int i = 0; i < contours.size(); i++)
		{

			if (contourArea(contours[i]) > 80) {
				
				sContourNumber << "(" << round(mc[i].x) << ";" << round(mc[i].y) << ")";
				sObjectNumber = sContourNumber.str();

				Point pCoordinates(mc[i].x + 5, mc[i].y - 7);
				Scalar color = Scalar(0,0,0);

				drawContours(frame, contours, i, color, 2, 8, hierarchy, 0, Point());
				circle(frame, mc[i], 4, color, -1, 8, 0);
				putText(frame, sObjectNumber, pCoordinates, FONT_HERSHEY_COMPLEX, 1, color, 1, 8);

				sContourNumber.str("");
				sContourNumber.clear();

			}
		}


		imshow("frame", frame);
		
		waitKey(1);
	}
	
	waitKey(0);
	return 0;
}