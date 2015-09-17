#ifndef BOTTLECUT_CPP_ 
#define BOTTLECUT_CPP_
#endif

#ifndef EDEGE_CPP_
#define EDEGE_CPP_
#endif

#ifndef MATCH_TEMPLATE_CPP_
#define MATCH_TEMPLATE_CPP_

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#endif

extern void BottleCut(Mat&, Mat&); //从背景中分离瓶身函数
extern void EdegeDetect(Mat&);   //边缘检测
extern void MatchTempl(Mat&, Mat&); //简单模板匹配
Mat img, result_r;
Point p1, p2, p3, p4;  //用于标记缺陷所在设置的边界点

int main()
{
	//img = imread("..\\TestImg\\IMG_20150813_122634.jpg");
	img = imread("..\\TestImg\\IMG_20150813_121501.jpg");
	imshow("src", img);
	BottleCut(img, result_r);
	EdegeDetect(result_r);
    
	Mat templ = imread("..\\TestImg\\t1.jpg");
	//imshow("templ", templ);
	MatchTempl(result_r, templ);
	waitKey(0);
}