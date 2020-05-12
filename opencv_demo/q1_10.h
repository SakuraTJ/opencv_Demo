#ifndef Q1_10_H
#define Q1_10_H
#include <string>
#include <core/core.hpp>

void q0(std::string filename);
cv::Mat channal_swap(cv::Mat img);
void q1(std::string filename);
cv::Mat grayscale(cv::Mat img);
void q2(std::string filename);
void thresholding(cv::Mat imggray, cv::Mat gray,int i);
//void changethresholding(cv::Mat imggray, cv::Mat gray,int i,int,void *);
void q3(std::string filename);
void q4(std::string filename);
cv::Mat hsv180r(cv::Mat img);
void q5(std::string filename);
cv::Mat compresscolor(cv::Mat img);
void q6(std::string filename);
cv::Mat poolavg(cv::Mat img);
void q7(std::string filename);
#endif // Q1_10_H
