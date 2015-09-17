#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


Mat result;
Mat t1, t2, t3, t4;
extern Mat img;
extern Point p1, p2, p3, p4;

void MatchTempl(Mat& img_1, Mat& templ)
{	
	/// 创建输出结果的矩阵
	int result_cols = img_1.cols - templ.cols + 1;
	int result_rows = img_1.rows - templ.rows + 1;

	img_1 = imread("..\\TestImg\\bcut.jpg");
	result.create(result_cols, result_rows, CV_32FC1);

	/// 进行匹配和标准化
	matchTemplate(img_1, templ,result, TM_CCORR_NORMED);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat()); 
	matchLoc = maxLoc;

	//尝试提取多个模板，效果不明显，于是只提取最佳匹配
	//for (int i = 0; i < result.rows; i++)
	//{
	//	for (int j = 0; j < result.cols; j++)
	//	{
	//		if (result.at<float>(i, j) >0.9)
	//		{
	//			matchLoc.x = i;
	//			matchLoc.y = j;
	//		}
	//		rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0,0,255), 1, 8, 0);
	//		rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0,0,255), 1, 8, 0);
	//	}
	//}
	Mat img_display;
	cvtColor(img, img_display, CV_GRAY2RGB);
	
	//标记模板匹配的轮廓
    rectangle(img_display, Point(matchLoc.x + p1.x, matchLoc.y + p1.y), Point(matchLoc.x + templ.cols + p1.x, matchLoc.y + templ.rows + p1.y), Scalar(0, 0, 255), 2, 8, 0);
	//标记 EdegeDetect 处理的轮廓
	rectangle(img_display, p3, p4, Scalar(0, 0, 255), 2, 8, 0);
	imshow("result", img_display);
	imwrite("..\\TestImg\\img_detect.jpg", img_display);
}