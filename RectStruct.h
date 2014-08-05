#pragma once
#include <opencv2/opencv.hpp>
#include <set>

using namespace std;
using namespace cv;

class RectStruct
{
	int number;
	Rect rectNow;
	set<int> points;
public:
	RectStruct(int, Rect, set<int>);
	set<int> getSetOfPoints();
	int getNumber();
	Rect getRect();
//	RectStruct();
	~RectStruct();

};

