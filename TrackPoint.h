#pragma once
#include "Frame.h"
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
using namespace chrono;

class TrackPoint
{
	milliseconds timeStamp;
	Rect rect;
public:
	TrackPoint(milliseconds, Rect);
	~TrackPoint();
	Point2f getCenter();

};

