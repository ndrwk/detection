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
	const int timeRange = 3000; // in milliseconds
	Mat frame;

public:
	Detect();
	~Detect();
	void Detect::detect(vector<Frame>&, mutex&);
	void Detect::training(vector<Frame>&, mutex&);
	void Detect::display();
};

