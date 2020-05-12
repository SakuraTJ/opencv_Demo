#include <iostream>
#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include "q1_10.h"
using namespace cv;

//≤‚ ‘∫Ø ˝
void q0(std::string filename)
{
	Mat img = imread(filename);
	namedWindow("≤‚ ‘Õº");
	imshow("≤‚ ‘Õº",img);
    waitKey(0);
}

//Õ®µ¿ÃÊªª RGB-?BGR
Mat channal_swap(Mat img)
{
	Mat out = Mat::zeros(img.rows,img.cols,CV_8UC3);

	for(int x=0;x<img.cols;x++)
	{
		for(int y=0;y<img.rows;y++)
		{
			out.at<Vec3b>(y,x)[0]=img.at<Vec3b>(y,x)[2];
			out.at<Vec3b>(y,x)[2]=img.at<Vec3b>(y,x)[0];
			out.at<Vec3b>(y,x)[1]=img.at<Vec3b>(y,x)[1];
		}
	}
	return out;

}
//Õ®µ¿ÃÊªª≤‚ ‘
void q1(std::string filename)
{
	Mat img = imread(filename);
	Mat imgnew = channal_swap(img);
	namedWindow("Õ®µ¿ÃÊªª");
	imshow("Õ®µ¿ÃÊªª",imgnew);
	waitKey(0);
}

