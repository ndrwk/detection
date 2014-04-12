#pragma once
#include <opencv2/opencv.hpp>
#include <mutex>
#include <chrono>
#include "Square.h"

using namespace cv;
using namespace std;
using namespace chrono;

class Capture
{
	VideoCapture cap;
	Mat frame;
	Mat mask;
	Mat fgimg;
	BackgroundSubtractorMOG2 bg;
	vector<vector<Point>> all_contours;
	milliseconds currentTime;

	bool isIntersected(Rect, Rect);
	void display();
	void displayTime(Mat);

public:
	Capture(int);
	~Capture();
	bool isOpened();
	Mat getFrame();
	void getFound(vector<Square>&, mutex&);

};

