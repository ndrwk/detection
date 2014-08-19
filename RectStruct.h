#pragma once
#include <opencv2/opencv.hpp>
#include <set>

using namespace std;
using namespace cv;

class RectStruct
{
	int number;
	Rect rectNow;
	vector<Point2f> points;
public:
//	RectStruct(int, Rect, set<int>);
	RectStruct(int, Rect, vector<Point2f>);
	vector<Point2f> getSetOfPoints();
	int getNumber();
	Rect getRect();
//	RectStruct();
	~RectStruct();

};

