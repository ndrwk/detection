#pragma once
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;
using namespace cv;

class Square
{
	chrono::milliseconds timeStamp;
	cv::Mat img;
	__int64 getHashValue(cv::Mat src);

public:
	Square(chrono::milliseconds, cv::Mat);
	~Square();
	__int64 getTime();
	cv::Mat getImg();
	__int64 getHash();

};

