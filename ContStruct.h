#pragma once
#include <opencv2/opencv.hpp>
#include <set>

using namespace std;
using namespace cv;

class ContStruct
{
	int number;
	vector<Point> contour;
	vector<Point2f> repers;

public:
	ContStruct(int, vector<Point>, vector<Point2f>);
	int getNumber();
	Rect getRect();
	~ContStruct();

};

