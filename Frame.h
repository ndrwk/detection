#pragma once
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;
using namespace cv;

class Frame
{
	chrono::milliseconds timeStamp;
	cv::Mat img;
	vector<Rect> allRects;

public:
	Frame(chrono::milliseconds, cv::Mat, vector<Rect>);
	~Frame();
	long long getTime();
	cv::Mat getImg();
	vector<Rect> getRects();
};

