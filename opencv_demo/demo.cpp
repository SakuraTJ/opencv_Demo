/****************
*本文件为非教程小算法实现demo
*****************/

#include <iostream>
#include <math.h>
#include<algorithm>
#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include "demo.h"
#include <opencv2/opencv.hpp>
#include "lpr.h"
using namespace cv;

/*
struct point
{
	int x;
	int y;
};
*/
//测试函数
void demo0(std::string filename)
{
	Mat img = imread(filename);
	namedWindow("测试图");
	imshow("测试图",img);
    waitKey(0);
}
/*
void line(Mat img,point p1,point p2,int width = 1)
{
	if (img.empty()) std::cout << "Error: img doesn't exist" << std::endl;
	
}
*/

//简单几何形状的绘制
void demo1()
{
	Mat out = Mat::zeros(600,800,CV_8U);

	//圆
	int r,rx,ry;
	r=234;
	rx=289;ry=304;

	for (int x = 0; x < out.cols; x++)
	{
		for (int y = 0; y < out.rows; y++)
		{
			if((x-rx)*(x-rx)+(y-ry)*(y-ry) <= r*r)
			{
				out.at<uchar>(y,x)=255;
			}
			
		}
	}

	//反相
	for (int x = 0; x < out.cols; x++)
	{
		for (int y = 0; y < out.rows; y++)
		{
			out.at<uchar>(y,x)=(out.at<uchar>(y,x)==255?0:255);
		}
	}
	namedWindow("demo");
	imshow("demo",out);
    waitKey(0);
}

//绘制坐标系
void demo2()
{
	Mat out(252,268,CV_8U,Scalar(255,255,255));

	line(out,Point(151,out.rows-107),Point(151,out.rows-189),Scalar(0,0,255),1,CV_AA);
	line(out,Point(151,out.rows-107),Point(68,out.rows-63),Scalar(0,0,255),1,CV_AA);
	line(out,Point(151,out.rows-107),Point(202,out.rows-63),Scalar(0,0,255),1,CV_AA);
	line(out,Point(151,out.rows-107),Point(184,out.rows-113),Scalar(0,0,255),1,CV_AA);

	namedWindow("demo");
	imshow("demo",out);
	//imwrite("G:/ADate/photo/axis.png",out);
    waitKey(0);
}

//车牌识别
void demo3()
{
	Mat test=LPRread(5);
	//imshow("过程图-原图",test);
	//waitKey(0);
	test=LPRgray(test);
	//imshow("过程图-灰度化",test);
	//waitKey(0);
	test=LPRfilter(test);
	//imshow("过程图-中值滤波",test);
	//waitKey(0);
	test=LPRhist(test);
	//imshow("过程图-直方图均一化",test);
	//waitKey(0);
	test=LPRedgesober(test);
	//imshow("过程图-边缘检测",test);
	//waitKey(0);
	test=LPRthreshold(test);
	//imshow("过程图-二值化",test);
	//waitKey(0);
	//test=LPRdilate(test);
	//test=LPRopde(test);
	//imshow("过程图-形态学操作",test);
	//waitKey(0);

	//LPRplotrect(test,LPRread(5));
	//waitKey(0);

	Mat test2=LPRcut(LPRread(5),LPRfindregion(test));
	test2=LPRstringcut(test2);
	namedWindow("demo");
	imshow("demo",test);
	imshow("提取的车牌",test2);
	waitKey(0);
}