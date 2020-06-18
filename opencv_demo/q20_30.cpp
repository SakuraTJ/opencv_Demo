#include <iostream>
#include <math.h>
#include<algorithm>
#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include "q20_30.h"
using namespace cv;

//直方图均一化
Mat hist_nomalization(Mat img,int a,int b)
{
	Mat out = Mat::zeros(img.rows, img.cols,CV_8UC3);
	int c=255,d=0,val;
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			  for (int c = 0; c < img.channels(); c++)
			  {
				val=img.at<Vec3b>(y,x)[c];
				c=(val<c?val:c);
				d=(val>d?val:d);
			   }
		}
    }

	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			  for (int c = 0; c < img.channels(); c++)
			  {
				val=img.at<Vec3b>(y,x)[c];
                if (val < a){
                    out.at<cv::Vec3b>(y, x)[c] = (uchar)a;
                }else if (val <= b)
				{
                    out.at<cv::Vec3b>(y, x)[c] = (uchar)((b - a) / (d - c) * (val - c) + a);
                } else 
				{
                    out.at<cv::Vec3b>(y, x)[c] = (uchar)b;
                 }
			  }
		}
    }

	return out;

}
