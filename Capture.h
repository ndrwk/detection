#pragma once
#include <opencv2/opencv.hpp>
#include <mutex>
#include <time.h>

using namespace cv;
using namespace std;

class Capture
{
	VideoCapture cap;
	Mat frame;
	Mat mask;
	Mat fgimg;
	BackgroundSubtractorMOG2 bg;
	vector<vector<Point>> all_contours;
	time_t currentTime;

	bool isIntersected(Rect, Rect);
	void display();
	void displayTime(Mat);

public:
	Capture(int);
	~Capture();
	bool isOpened();
	Mat getFrame();
	void getFound(map<time_t,Mat>&, mutex&);

};

