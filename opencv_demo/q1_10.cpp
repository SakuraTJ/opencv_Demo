#include <iostream>
#include <math.h>
#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include "q1_10.h"
using namespace cv;

//���Ժ���
void q0(std::string filename)
{
	Mat img = imread(filename);
	namedWindow("����ͼ");
	imshow("����ͼ",img);
    waitKey(0);
}

//ͨ���滻 RGB->BGR
Mat channal_swap(Mat img)
{
	Mat out = Mat::zeros(img.rows, img.cols,CV_8UC3);

	for (int x = 0; x < img.cols; x++)
	{
		for (int y = 0; y < img.rows; y++)
		{
			out.at<Vec3b>(y, x)[0] = img.at<Vec3b>(y, x)[2];
			out.at<Vec3b>(y, x)[2] = img.at<Vec3b>(y, x)[0];
			out.at<Vec3b>(y, x)[1] = img.at<Vec3b>(y, x)[1];
		}
	}
	return out;
}

//ͨ���滻����
void q1(std::string filename)
{
	Mat img = imread(filename);
	Mat imgnew = channal_swap(img);
	namedWindow("ͨ���滻");
	imshow("ͨ���滻",imgnew);
	waitKey(0);
}

//�ҶȻ�(3ͨ����
Mat grayscale(Mat img)
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

//�ҶȻ����� + ROI
void q2(std::string filename)
{
	Mat img = imread(filename);
	Mat imgnew = grayscale(img);
	namedWindow("�ҶȻ�");
	imshow("�ҶȻ�",imgnew);
	waitKey(0);
	Mat img2 = Mat(img.rows,img.cols * 2,CV_8UC3);
	Mat roiright = img2(Rect(0,0,img.cols,img.rows));
	img.copyTo(roiright);
	Mat roileft = img2(Rect(img.cols,0,img.cols,img.rows));
	imgnew.convertTo(imgnew,CV_8UC3);
	imgnew.copyTo(roileft);
	namedWindow("�ҶȻ�");
	imshow("�ҶȻ�",img2);
	waitKey(0);
}

//��ֵ����ʾ
void thresholding(Mat imggray,Mat gray,int i)
{
	for (int x = 0; x < imggray.cols; x++)
	{
		for (int y = 0; y < imggray.rows; y++)
		{
			gray.at<uchar>(y,x) = imggray.at<Vec3b>(y, x)[2] > i ? 255 : 1;
		}
	}
	//imshow("��ֵ��",gray);
}

//��ֵ���ı���ֵ
//void changethresholding(int i,void *)
//{
//	thresholding(imggray,gray,i);
//}

//��ֵ������ 
void q3(std::string filename)
{
	Mat img = imread(filename);
	Mat imggray = grayscale(img);
	Mat gray = Mat(imggray.rows,imggray.cols,CV_8UC1);
	int i=128;
	string name = "��ֵ��,��ֵ��"+ std::to_string(i);
	namedWindow(name);
	thresholding(imggray,gray,i);
	imshow(name,gray);
	//createTrackbar("��ֵ��", "��ֵ��", &i, 255, changethresholding);
	//changethresholding(imggray,gray,i,0);
	waitKey(0);
}

//���ɶ�ֵ���㷨
void q4(std::string filename)
{
	Mat img = imread(filename);
    if (img.empty())
    {
        std::cout << "Error: Could not load image" << std::endl;
    }

    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);

    Mat dst;
    threshold(gray, dst, 0, 255, CV_THRESH_OTSU);//ֱ�ӵ����㷨

    imshow("src", img);
    imshow("gray", gray);
    imshow("dst", dst);
    waitKey(0);
}

//HSV���� ɫ�෴��
Mat hsv180r(Mat img)
{
	Mat out = Mat::zeros(img.rows, img.cols,CV_8UC3);
	cvtColor(img,out,CV_BGR2HSV);
	namedWindow ("hsv");
	imshow("hsv",out);
	for (int x = 0; x < img.cols; x++)
	{
		for (int y = 0; y < img.rows; y++)
		{
			int h = out.at<Vec3b>(y,x)[0] * 2 + 180;
			out.at<Vec3b>(y,x)[0] = h >=360 ? (uchar)(h-360)/2:(uchar)h/2;
			
		}
	}
	cvtColor(out,out,CV_HSV2BGR);
	return out;
}

//ɫ�෴�����
void q5(std::string filename)
{
	Mat img = imread(filename);
	Mat imgnew = hsv180r(img);
	Mat img2 = Mat(img.rows,img.cols * 2,CV_8UC3);
	Mat roiright = img2(Rect(0,0,img.cols,img.rows));
	img.copyTo(roiright);
	Mat roileft = img2(Rect(img.cols,0,img.cols,img.rows));
	imgnew.convertTo(imgnew,CV_8UC3);
	imgnew.copyTo(roileft);
	namedWindow("����");
	imshow("����",img2);
	waitKey(0);
}

//��ɫ��������
int compress4(int num)
{
	if(num<64)
		return 32;
	else if(num<128)
		return 96;
	else if(num<192)
		return 160;
	else
		return 224;
}
//��ɫ���� ѹ��
Mat compresscolor(Mat img)
{
	Mat out = Mat::zeros(img.rows, img.cols,CV_8UC3);
	for (int x = 0; x < img.cols; x++)
	{
		for (int y = 0; y < img.rows; y++)
		{
			out.at<Vec3b>(y, x)[2] = compress4(img.at<Vec3b>(y, x)[2]);
			out.at<Vec3b>(y, x)[0] = compress4(img.at<Vec3b>(y, x)[0]);
			out.at<Vec3b>(y, x)[1] = compress4(img.at<Vec3b>(y, x)[1]);
		}
	}
	return out;
}

//��ɫ����
void q6(std::string filename)
{
	Mat img = imread(filename);
	Mat imgnew = compresscolor(img);
	Mat img2 = Mat(img.rows,img.cols * 2,CV_8UC3);
	Mat roiright = img2(Rect(0,0,img.cols,img.rows));
	img.copyTo(roiright);
	Mat roileft = img2(Rect(img.cols,0,img.cols,img.rows));
	imgnew.convertTo(imgnew,CV_8UC3);
	imgnew.copyTo(roileft);
	namedWindow("��ɫ");
	imshow("��ɫ",img2);
	waitKey(0);
}

//ƽ���ػ�
Mat poolavg(Mat img)
{
	Mat out = Mat::zeros(img.rows, img.cols,CV_8UC3);
	const int poolpara=10; //�����С

	for (int x = 0; x < img.cols ; x+=poolpara)
	{
		for (int y = 0; y < img.rows ; y+=poolpara)
		{
			int v;
			for (int c = 0; c < 3; c++){
				v = 0;
				for (int dy = 0; dy < poolpara; dy++){
					 for (int dx = 0; dx < poolpara; dx++){
						v += (double)img.at<Vec3b>(y + dy, x + dx)[c];
					 }
				}
				v /= (poolpara * poolpara);
				for (int dy = 0; dy < poolpara; dy++){
					for (int dx = 0; dx < poolpara; dx++){
						out.at<Vec3b>(y + dy, x + dx)[c] = (uchar)v;
					}
				 }
			}
		}
	}
	return out;
}

//ƽ���ػ�����
void q7(std::string filename)
{
	Mat img = imread(filename);
	Mat imgnew = poolavg(img);
	Mat img2 = Mat(img.rows,img.cols * 2,CV_8UC3);
	Mat roiright = img2(Rect(0,0,img.cols,img.rows));
	img.copyTo(roiright);
	Mat roileft = img2(Rect(img.cols,0,img.cols,img.rows));
	imgnew.convertTo(imgnew,CV_8UC3);
	imgnew.copyTo(roileft);
	namedWindow("ƽ���ػ�");
	imshow("ƽ���ػ�",img2);
	waitKey(0);
}

//����������
Mat randnoise(Mat img)
{
	int noise_num=3000;//�����Ŀ
	Mat out = img.clone();
	int x,y;
	for(int i=0;i<noise_num;i++)
	{
		x=rand() % img.cols;
		y=rand() % img.rows;
		out.at<Vec3b>(y, x)[2] = 0;
		out.at<Vec3b>(y, x)[0] = 0;
		out.at<Vec3b>(y, x)[1] = 0;
	}
	namedWindow("����ͼ");
	imshow("����ͼ",out);
	return out;
}

//��˹�˲���
Mat gaussfilter(Mat img)
{
	//�����
	auto sigma=1.3;
	const int kernel_size = 3;//����˴�С
	const auto PI = 3.1415926;
	int pad = floor(kernel_size / 2);
	auto _x = 0, _y = 0;
    double kernel_sum = 0;
 
	float kernel[kernel_size][kernel_size];

	for (int y = 0; y < kernel_size; y++){
		for (int x = 0; x < kernel_size; x++){
			_y = y - pad;
			_x = x - pad; 
			kernel[y][x] = 1 / (2 * PI * sigma * sigma) * exp( - (_x * _x + _y * _y) / (2 * sigma * sigma));
			kernel_sum += kernel[y][x];
		}
	}
	for (int y = 0; y < kernel_size; y++){
		for (int x = 0; x < kernel_size; x++){
			 kernel[y][x] /= kernel_sum;
		}
	}

	// �˲�
	double v = 0;
    Mat out = Mat::zeros(img.rows, img.cols,CV_8UC3);
	
	for (int y = 0; y < img.rows-pad; y++)
	{
		for (int x = 0; x < img.cols -pad; x++)
		{
			  for (int c = 0; c < img.channels(); c++)
			  {
			  	v = 0;
				for (int dy = -pad; dy < pad + 1; dy++){
					for (int dx = -pad; dx < pad + 1; dx++){
						if (((x + dx) >= 0) && ((y + dy) >= 0)){ //padding zero
							v += (double)img.at<Vec3b>(y + dy, x + dx)[c] * kernel[dy + pad][dx + pad];
						}
					}
				}
				out.at<Vec3b>(y, x)[c] = (uchar)v;
			  }
		}
    }
	return out;
}

//��˹�˲�����
void q8(std::string filename)
{
	Mat img = imread(filename);
	Mat imgnoise = randnoise(img);
	Mat imgnew = gaussfilter(imgnoise);
	Mat img2 = Mat(img.rows,img.cols * 2,CV_8UC3);
	Mat roiright = img2(Rect(0,0,img.cols,img.rows));
	img.copyTo(roiright);
	Mat roileft = img2(Rect(img.cols,0,img.cols,img.rows));
	imgnew.convertTo(imgnew,CV_8UC3);
	imgnew.copyTo(roileft);
	namedWindow("��˹�˲�");
	imshow("��˹�˲�",img2);
	waitKey(0);
}