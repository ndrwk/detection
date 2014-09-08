#pragma once
#include <opencv2/opencv.hpp>
#include <chrono>
#include "TrackPoint.h"

using namespace std;
using namespace cv;

class Frame
{
	chrono::milliseconds timeStamp;
	Mat img, mask;

public:
	Frame(chrono::milliseconds, Mat, Mat);
	~Frame();
	long long getTime();
	Mat getImg();
	Mat getMask();
};

