#pragma once
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;
using namespace cv;

class Frame
{
	chrono::milliseconds timeStamp;
	cv::Mat img;
	vector<vector<Point>> allContours;

public:
	Frame(chrono::milliseconds, cv::Mat, vector<vector<Point>>);
	~Frame();
	long long getTime();
	cv::Mat getImg();
	vector<vector<Point>> getContours();

};

