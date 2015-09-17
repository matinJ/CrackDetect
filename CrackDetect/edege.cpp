#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/nonfree/features2d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <vector>
using namespace std;
using namespace cv;

extern Mat img;
extern Point p1, p2, p3, p4;

void EdegeDetect(Mat& img_1)
{
	Mat result;
	Canny(img_1, result, 50,200);
	//imshow("re", result);
	//dilate(result, result, Mat(), Point(-1, -1), 2);
	//erode(result, result, Mat(), Point(-1, -1), 1);

	//对提取出的瓶身图像做黑帽操作
	morphologyEx(result, result, MORPH_BLACKHAT, Mat(3, 3, CV_8U), Point(-1, -1), 2);
	//imshow("erode", result);

	//做最大连通域提取
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(result, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
    double contour_area_temp = 0, contour_area_max = 0;
	int maxi = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}
	Mat drawing = Mat::zeros(result.size(), CV_8UC3);
	//cout << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		contour_area_temp = boundRect[i].height * boundRect[i].width;
		if (contour_area_temp > contour_area_max)
		{
			maxi = i;
			contour_area_max = contour_area_temp;
		}
	}
	Scalar color = Scalar(0, 0, 255);
	drawContours(drawing, contours_poly, maxi, color, 1, 8, vector<Vec4i>(), 0, Point());
	p3.x = boundRect[maxi].tl().x + p1.x;
	p3.y = boundRect[maxi].tl().y + p1.y;
	p4.x = boundRect[maxi].br().x + p1.x;
	p4.y = boundRect[maxi].br().y + p1.y;
	//rectangle(img, p3, p4, color, 2, 8, 0)
	//imshow("result", img); 
}