#ifndef LPR_H
#define LPR_H
#include <string>
#include <core/core.hpp>

cv::Mat LPRread();
cv::Mat LPRgray(cv::Mat img);
cv::Mat LPRfilter(cv::Mat img);
cv::Mat LPRhist(cv::Mat img);
cv::Mat LPRthreshold(cv::Mat img);

#endif // LPR_H
