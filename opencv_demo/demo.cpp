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
	Mat test=LPRread();
	test=LPRgray(test);
	test=LPRfilter(test);
	test=LPRhist(test);

	Mat img2 = Mat(test.rows,test.cols * 2,CV_8UC3);
	Mat roiright = img2(Rect(0,0,test.cols,test.rows));
	test.copyTo(roiright);
	
	test=LPRthreshold(test);
	Mat roileft = img2(Rect(test.cols,0,test.cols,test.rows));
	test.copyTo(roileft);

	namedWindow("demo");
	imshow("demo",img2);
	waitKey(0);
}