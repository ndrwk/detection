#pragma once
#include <opencv2/opencv.hpp>
#include <chrono>
#include "RectStruct.h"

using namespace std;
using namespace cv;

class Frame
{
	chrono::milliseconds timeStamp;
	Mat img, fgimg;
	vector<Rect> allRects;
	vector<Point2f> allPoints;
	vector<RectStruct> numberedRects;
//	vector <vector<Point>> allContours;

public:
	Frame(chrono::milliseconds, Mat, vector<Rect>, Mat);
	~Frame();
	long long getTime();
	Mat getImg();
	Mat getFgimg();
	vector<Rect> getRects();
//	vector<vector<Point>> getContours();
	vector<Point2f> getPoints();
};

