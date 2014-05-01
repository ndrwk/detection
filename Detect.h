#pragma once
#include <opencv2/opencv.hpp>
#include <mutex>
#include <vector>
#include <chrono>
#include "Frame.h"


using namespace cv;
using namespace std;
using namespace chrono;

class Detect
{

public:
	Detect();
	~Detect();
	void Detect::detectOnContours(vector<Frame>&, mutex&);
	void Detect::detectOnRects(vector<Frame>&, mutex&);
	void Detect::display(Mat);
};

