#pragma once
#include <opencv2/opencv.hpp>
#include <mutex>
#include <vector>
#include <time.h>


using namespace cv;
using namespace std;

class Detect
{
	__int64 Detect::getHashValue(Mat src);
	__int64 Detect::calcHammingDistance(__int64 x, __int64 y);

public:
	Detect();
	~Detect();
	void Detect::detection(map<time_t,Mat>&, mutex&);
};

