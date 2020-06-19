//License Plate Recognition
//����ʶ��
//�ؼ�����

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
Mat C3toC1(Mat dst);
Mat C1toC3(Mat dst);
void histshow(Mat img);

//��ȡ����ͼ��
Mat LPRread(int j)
{
	string defaultpath="G:/ADate/photo/lpr";
	vector<String> file_vec;
	glob(defaultpath,file_vec,false);
	int i=0;
	std::cout<<"[���]    ͼƬ·��"<<std::endl;
	for (string filename : file_vec)
	{
		std::cout<<" ["<<i++<<"]      "<<filename<< std::endl;
	}
	if(j==-1 || j>=file_vec.size())
	{
	    std::cout<<"����Ҫ��ͼƬ�����:"<< std::endl;
	    std::cin>>j;
	}else
	{
		std::cout<<"��["<<j<<"]"<< std::endl;
	}
	

	Mat out = imread(file_vec[j]);
	return out;
}

//�ҶȻ�-��Ȩƽ����
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

//��ֵ�˲�-ȥ����������
Mat LPRfilter(Mat img)
{
	Mat out;
	medianBlur(img,out,3);
	return out;
}

//ֱ��ͼ���⻯,��չ��[0,255]
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

//ͼ���ֵ��
Mat LPRthreshold(Mat img)
{
	Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);
	Mat dst;
	threshold(gray, dst, 0, 255, CV_THRESH_OTSU);//���ɶ�ֵ���㷨

	//��ͨ��-����ͨ��
	Mat out=C1toC3(dst);
	return  out;
}

//��Ե���-Sober����
Mat LPRedgesober(Mat img)
{
	Mat kernelx=(Mat_<char>(3,3)<<
		1,0,-1,
		2,0,-2,
		1,0,-1);
	Mat kernely=(Mat_<char>(3,3)<<
		1,2,1,
		0,0,0,
		-1,-2,-1);
	Mat out1,out2,out;
    filter2D(img,out1,img.depth(),kernelx);
	filter2D(img,out2,img.depth(),kernely);
	addWeighted(out1,0.5,out2,0.5,0,out);
	return out;
}

//��Ե���-maxmin�˲���(�����ٶ���)
Mat LPRedgemaxmin(Mat img)
{
	//�����
	const int kernel_size = 3;//����˴�С �޶�����
    double kernel_sum = 0;
    int pad = floor(kernel_size / 2);
	vector<int> kernel;
	
	// �˲�
    Mat out = Mat::zeros(img.rows, img.cols,CV_8UC3);
	for (int y = 0; y < img.rows-pad; y++)
	{
		for (int x = 0; x < img.cols -pad; x++)
		{
			  for (int c = 0; c < img.channels(); c++)
			  {
				for (int dy = -pad; dy < pad + 1; dy++){
					for (int dx = -pad; dx < pad + 1; dx++){
						if (((x + dx) >= 0) && ((y + dy) >= 0)){ 
							kernel.push_back(img.at<Vec3b>(y + dy, x + dx)[c]);
						}else
						{
							kernel.push_back(0);//padding zero
						}
					}
				}
				sort(kernel.begin(),kernel.end());
			    out.at<Vec3b>(y, x)[c] = (uchar)(kernel.back()-kernel.front());
				kernel.clear();
			  }
		}
    }
	return out;
}


//���ҳ��Ʊ�Ե������λ��
Point2f* LPRfindregion(Mat img)
{
	Point2f *region = new Point2f[4];
	vector<vector<Point>> vContours;
	img=C3toC1(img);
	Mat mContourShow = Mat::zeros(img.size(),CV_8U);
	findContours(img,vContours,RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	for (vector<Point> vcontour : vContours)
	{
		double area=contourArea(vcontour);
		if(area<400)//ȥ�������С������
		{
			continue;
		}

		// ��С�������
        RotatedRect rrt = minAreaRect(vcontour);
        Point2f pts[4];
        rrt.points(pts);
        // ������ת����������λ��
		/*
        for (int i = 0; i < 4; i++) {
            line(out, pts[i % 4], pts[(i + 1) % 4], Scalar(255), 1, 8, 0);
        }
        Point2f cpt = rrt.center;
        circle(out, cpt, 2, Scalar(255, 0, 0), 2, 8, 0);
		*/
		//������γ���
		double height=sqrt((pts[0].x-pts[1].x)*(pts[0].x-pts[1].x)+(pts[0].y-pts[1].y)*(pts[0].y-pts[1].y));
		double width=sqrt((pts[2].x-pts[1].x)*(pts[2].x-pts[1].x)+(pts[2].y-pts[1].y)*(pts[2].y-pts[1].y));
		if(height>width)
		{
			height+=width;
			width=height-width;
			height-=width;
		}
		double ratio=width/height;
		if(ratio<5 && ratio>2)
		{
			for(int i=0;i<4;i++) region[i]=pts[i];
			break;
		}
	}
	return region;
}


//��ԭͼ�ϻ���������Ӿ���
void LPRplotrect(Mat img,Mat img0)
{
	vector<vector<Point>> vContours;
	vector<vector<Point>> vContours2;
	img=C3toC1(img);
    Mat mContourShow = Mat::zeros(img.size(),CV_8U);
    findContours(img,vContours,RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	Mat out=img0.clone();
	for (vector<Point> vcontour : vContours)
	{
		double area=contourArea(vcontour);
		if(area<1)//ȥ�������С������
		{
			continue;
		}
		vContours2.push_back(vcontour);

		// ��С�������
        RotatedRect rrt = minAreaRect(vcontour);
        Point2f pts[4];
        rrt.points(pts);
        
		//������γ���
		double height=sqrt((pts[0].x-pts[1].x)*(pts[0].x-pts[1].x)+(pts[0].y-pts[1].y)*(pts[0].y-pts[1].y));
		double width=sqrt((pts[2].x-pts[1].x)*(pts[2].x-pts[1].x)+(pts[2].y-pts[1].y)*(pts[2].y-pts[1].y));
		if(height>width)
		{
			height+=width;
			width=height-width;
			height-=width;
		}
		double ratio=width/height;
		if(ratio<5 && ratio>2)
		{
			// ������ת����������λ��
	
			for (int i = 0; i < 4; i++) {
			 line(out, pts[i % 4], pts[(i + 1) % 4], Scalar(0,0,255), 2, 8, 0);
			  }
			 Point2f cpt = rrt.center;
			circle(out, cpt, 2, Scalar(255, 0, 0), 2, 8, 0);
		
		}

	}
    drawContours(mContourShow,vContours2,-1,Scalar(255));
	imshow("������",mContourShow);
	imshow("Ŀ���������",out);
}


//��̬ѧ����-����(�ֲ����ֵ)
Mat Dilate(Mat img)
{
	//img ��ͨ��

	//�ṹԪ��
	int SE_kernel[3][3]={{0,1,0},{1,1,1},{0,1,0}};
	int pad=1;
	Mat out;
	img.copyTo(out);
	
	for (int y = 0; y < img.rows-pad; y++)
	{
		for (int x = 0; x < img.cols -pad; x++)
		{
			  	int v = 0;
				for (int dy = -pad; dy < pad + 1; dy++){
					for (int dx = -pad; dx < pad + 1; dx++){
						if (((x + dx) >= 0) && ((y + dy) >= 0)){ //padding zero
							v += (int)img.at<uchar>(y + dy, x + dx) * SE_kernel[dy + pad][dx + pad];
						}
					}
			    }
			if(v>0) out.at<uchar>(y, x) = 255;
		}
    }
	return out;
}

//��̬ѧ����-��ʴ(�ֲ���Сֵ)
Mat Erode(Mat img)
{
	//img ��ͨ��

	//�ṹԪ��
	int SE_kernel[3][3]={
		{0,1,0},
		{1,1,1},
		{0,1,0}};
	int pad=1;
	Mat out;
	img.copyTo(out);

	for (int y = 0; y < img.rows-pad; y++)
	{
		for (int x = 213; x < img.cols -pad; x++)
		{
			  	int v = 0;

				    for (int dy = -pad; dy < pad + 1; dy++){
					for (int dx = -pad; dx < pad + 1; dx++){
						if (((x + dx) >= 0) && ((y + dy) >= 0)){ //padding zero
							v += (int)img.at<uchar>(y + dy, x + dx) * SE_kernel[dy + pad][dx + pad];
						}
					}
			        }
			        if(v<1275) out.at<uchar>(y, x) = 0;	
		}
		
    }
	
	return out;
}

//��ʴ
Mat LPRerode(Mat img)
{
	Mat dst=C3toC1(img);
	Mat dst2;
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
	erode(dst, dst2, element);
	Mat out=C1toC3(dst2);
	return out;
}

//����
Mat LPRdilate(Mat img)
{
	Mat dst=C3toC1(img);
	Mat dst2;
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
	dilate(dst, dst2, element);
	Mat out=C1toC3(dst2);
	return out;
}

//������ �ȸ�ʴ������-����С����(�ף�
Mat LPRoped(Mat img)
{
	Mat out=LPRerode(img);
	return LPRdilate(out);
}

//������ �������ٸ�ʴ-���С��϶���ڣ�
Mat LPRopde(Mat img)
{
	Mat out=LPRdilate(img);
	return LPRerode(out);
}


//���Ʒָ�
Mat LPRcut(Mat img,Point2f *region)
{
	//img:ԭͼ
	//region�ָ���������
	float xmin=region[0].x,xmax=region[0].x,ymin=region[0].y,ymax=region[0].y;
	for (int i = 0; i < 4; i++) {
            xmin=(region[i].x<xmin?region[i].x:xmin);
		    xmax=(region[i].x>xmax?region[i].x:xmax);
		    ymin=(region[i].y<ymin?region[i].y:ymin);
		    ymax=(region[i].y>ymax?region[i].y:ymax);
        }
	Mat out=img(Range((int)ymin,(int)ymax),Range((int)xmin,(int)xmax)).clone();
	delete region;
	return out;
}

//�ַ��ָ�
Mat LPRstringcut(Mat img)
{
	Mat carlic=img.clone();
	carlic=LPRgray(carlic);
	carlic=LPRthreshold(carlic);
	histshow(carlic);
	return carlic;
}


//��ͨ��->��ͨ��
Mat C1toC3(Mat dst)
{
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

//�Ҷ�ͼ����ͨ��->��ͨ��
Mat C3toC1(Mat dst)
{
	Mat out=Mat(dst.rows,dst.cols,CV_8UC1);
	for (int y = 0; y < dst.rows; y++)
	{
		for (int x = 0; x < dst.cols; x++)
		{
				  out.at<uchar>(y,x)=dst.at<Vec3b>(y,x)[0];
		}
    }
	return  out;
}

//����ƽ����


//��ͨ��ͼ���������ظ���ֱ��ͼ
void histshow(Mat img)
{
	Mat img0=img.clone();
	if(img0.type()==16) img0=C3toC1(img0);
	vector<int> data;
	int ymax=0;
	for(int x=0;x<img0.cols;x++)
	{
		int sum=0;
		for (int y = 0; y < img0.rows; y++)
		{
			sum+=img0.at<uchar>(y,x);
		}
		int p=img0.rows-sum/255;
		data.push_back(p);
		ymax=(p>ymax?p:ymax);
	}
	ymax=static_cast<int>(ymax * 1.2);
	Mat plot=Mat::zeros(ymax,data.size(),CV_8U);
	for(int i=0;i<data.size()-1;i++)
	{
		line(plot,Point(i,data[i]),Point(i+1,data[i+1]),Scalar(255,255,255),1,8);
	}
	imshow("������������ͼ",plot);

}






/*
 
 try{

  }
catch (std::exception e)
{
    std::cout << e.what()<< std::endl;
    system("pause");
}

*/

//lpath ���ڱ���ͼƬ�����ļ��е�����
void getFiles(string path, vector<string> &files, string lpath)
{
    //�ļ����
    long hFile = 0;
    //�ļ���Ϣ
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
