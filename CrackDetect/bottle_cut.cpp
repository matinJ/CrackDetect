#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/nonfree/features2d.hpp"
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

extern Point p1, p2;
void BottleCut(Mat& img_1,Mat& result_r)
{
	int sigma = 3;
	Mat result, dst;
	cvtColor(img_1, img_1, CV_RGB2GRAY);
	GaussianBlur(img_1, dst,Size(3,3), sigma,sigma);

//	morphologyEx(dst, dst, MORPH_OPEN, Mat(3, 3, CV_8U), Point(-1, -1), 1);
	Mat threshold_out;
	threshold(dst, threshold_out, 145, 255, CV_THRESH_BINARY);  

	//阈值分割之后寻找边缘 用于分割瓶身
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(threshold_out, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	double contour_area_temp = 0, contour_area_max = 0;   //找到最大的矩形匹配框
	int flag = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}
	Mat drawing = Mat::zeros(threshold_out.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		contour_area_temp = boundRect[i].height * boundRect[i].width; //获取当前轮廓面积
		if (contour_area_temp > contour_area_max)
		{
			contour_area_max = contour_area_temp;
			flag = i;			
		}
	}
	Scalar color = Scalar(255, 0, 0);
//	drawContours(drawing, contours_poly, flag, color, 1, 8, vector<Vec4i>(), 0, Point());
	//rectangle(img_1, boundRect[flag].tl(), boundRect[flag].br(), color, 2, 8, 0);
	
	//分割瓶身，为减少干扰，将瓶颈部和瓶底部分复杂的轮廓干掉
	p1.x = boundRect[flag].tl().x + (boundRect[flag].br().x - boundRect[flag].tl().x) / 20;
	p1.y = boundRect[flag].tl().y + (boundRect[flag].br().y - boundRect[flag].tl().y) / 20;
	p2.x = boundRect[flag].br().x - (boundRect[flag].br().x - boundRect[flag].tl().x) / 20;
	p2.y = boundRect[flag].br().y - (boundRect[flag].br().y - boundRect[flag].tl().y) / 20;

	//Mat rr (dst,Rect(boundRect[flag].tl(), boundRect[flag].br()));
	result_r = img_1( Rect(p1, p2));
	imwrite("..\\TestImg\\bcut.jpg",result_r);   //分离的瓶身
	//imshow("contours", img_1);
}
