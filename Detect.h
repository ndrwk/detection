#pragma once
#include <opencv2/opencv.hpp>
#include <mutex>
#include <vector>
#include <set>
#include <chrono>
#include "Frame.h"
#include "ContStruct.h"


using namespace cv;
using namespace std;
using namespace chrono;


class Detect
{
	const Scalar BLACK = CV_RGB(0, 0, 0);
	const Scalar WHITE = CV_RGB(255, 255, 255);
	const Scalar RED = CV_RGB(255, 0, 0);
	const Scalar ORANGE = CV_RGB(255, 100, 0);
	const Scalar YELLOW = CV_RGB(255, 255, 0);
	const Scalar GREEN = CV_RGB(0, 255, 0);
	const Scalar LIGHTBLUE = CV_RGB(60, 170, 255);
	const Scalar BLUE = CV_RGB(0, 0, 255);
	const Scalar VIOLET = CV_RGB(194, 0, 255);
	const Scalar GINGER = CV_RGB(215, 125, 49);
	const Scalar PINK = CV_RGB(255, 192, 203);
	const Scalar LIGHTGREEN = CV_RGB(153, 255, 153);
	const Scalar BROWN = CV_RGB(150, 75, 0);

	const int MAXCORNERS = 500;

	float Detect::getVortex(float, float);
	float Detect::getAngle(float, float);

public:
	Detect();
	~Detect();
	void Detect::detectPoints(vector<Frame>&, mutex&);
	void Detect::d_etectPoints(vector<Frame>&, mutex&);
	void Detect::display(vector<Frame>&, mutex&);

};

