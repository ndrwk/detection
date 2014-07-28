#pragma once
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class RectStruct
{
	int number;
	Rect rect;

public:
	RectStruct(int, Rect);
	~RectStruct();
};

