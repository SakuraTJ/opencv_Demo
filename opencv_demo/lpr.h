#ifndef LPR_H
#define LPR_H
#include <string>
#include <core/core.hpp>

cv::Mat LPRread(int j = -1);
cv::Mat LPRgray(cv::Mat img);
cv::Mat LPRfilter(cv::Mat img);
cv::Mat LPRhist(cv::Mat img);
cv::Mat LPRthreshold(cv::Mat img);
cv::Mat LPRedgesober(cv::Mat img);
cv::Mat LPRedgemaxmin(cv::Mat img);
cv::Mat LPRerode(cv::Mat img);
cv::Mat LPRdilate(cv::Mat img);
cv::Mat LPRoped(cv::Mat img);
cv::Mat LPRopde(cv::Mat img);
void LPRplotrect(cv::Mat img, cv::Mat img0);
cv::Point2f* LPRfindregion(cv::Mat img);
cv::Mat LPRcut(cv::Mat img, cv::Point2f *region);
cv::Mat LPRstringcut(cv::Mat img);
#endif // LPR_H
