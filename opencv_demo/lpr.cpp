//License Plate Recognition
//车牌识别
//关键函数

#include <iostream>
#include <math.h>
#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include "lpr.h"
#include <opencv2/opencv.hpp>
#include <io.h>

using namespace cv;

void getFiles(string path, vector<string> &files, string lpath);


//读取车牌图像
Mat LPRread()
{
	string defaultpath="G:/ADate/photo/lpr";
	vector<String> file_vec;
	glob(defaultpath,file_vec,false);
	int i=0;
	std::cout<<"[序号]    图片路径"<<std::endl;
	for (string filename : file_vec)
	{
		std::cout<<" ["<<i++<<"]      "<<filename<< std::endl;
	}
	std::cout<<"输入要打开图片的序号:"<< std::endl;
	std::cout<<"5"<<std::endl;
	//std::cin>>i;

	Mat out = imread(file_vec[5]);
	return out;
}

//灰度化-加权平均法
Mat LPRgray(Mat img)
{
	Mat out = Mat(img.rows,img.cols,CV_8UC3);
	for (int x = 0; x < img.cols; x++)
	{
		for (int y = 0; y < img.rows; y++)
		{
			out.at<Vec3b>(y,x)[0] = (uchar)(img.at<Vec3b>(y, x)[2] * 0.2126 + img.at<Vec3b>(y, x)[1] * 0.7152 + img.at<Vec3b>(y, x)[0] * 0.0722);
			out.at<Vec3b>(y,x)[1]=out.at<Vec3b>(y,x)[0];
			out.at<Vec3b>(y,x)[2]=out.at<Vec3b>(y,x)[0];
		}
	}
	return out;
}

//中值滤波-去除椒盐噪声
Mat LPRfilter(Mat img)
{
	Mat out;
	medianBlur(img,out,3);
	return out;
}

//直方图均衡化,扩展到[0,255]
Mat LPRhist(Mat img)
{
	Mat out = Mat::zeros(img.rows, img.cols,CV_8UC3);
	int c=255,d=0,val;
	int a=0,b=255;
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

//图像二值化
Mat LPRthreshold(Mat img)
{
	Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);
	Mat dst;
	threshold(gray, dst, 0, 255, CV_THRESH_OTSU);//大律二值化算法

	//单通道-》三通道
	Mat out=Mat(dst.rows,dst.cols,CV_8UC3);
	for (int y = 0; y < dst.rows; y++)
	{
		for (int x = 0; x < dst.cols; x++)
		{
			  for (int c = 0; c < 3; c++)
			  {
				  out.at<Vec3b>(y,x)[c]=dst.at<uchar>(y,x);
			  }
		}
    }
	return  out;
}

//形态学操作





//lpath 用于保存图片所在文件夹的名字
void getFiles(string path, vector<string> &files, string lpath)
{
    //文件句柄
    long hFile = 0;
    //文件信息
    printf("lpath %s\n",lpath.c_str());
    struct _finddata_t fileinfo;
    string p;
    string pl;
    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
    {
        string pathTemp;
        do {

            if (fileinfo.attrib == _A_SUBDIR)
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                {
                    pathTemp = fileinfo.name;
                    getFiles(p.assign(path).append("/").append(fileinfo.name), files
                        , p.assign(lpath).append("/").append(fileinfo.name));
                     printf("XXX  %s\n", pl.c_str());
                }
            }
            else
            {
                pathTemp = lpath;
                files.push_back(pathTemp.append("/").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }

}
