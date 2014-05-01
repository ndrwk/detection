#pragma once
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;
using namespace cv;

class Frame
{
	chrono::milliseconds timeStamp;
	Mat img, fgimg;
	vector<Rect> allRects;
	vector <vector<Point>> allContours;

public:
	Frame(chrono::milliseconds, Mat, vector<Rect>, Mat, vector<vector<Point>>);
	~Frame();
	long long getTime();
	Mat getImg();
	Mat getFgimg();
	vector<Rect> getRects();
	vector<vector<Point>> getContours();
};

