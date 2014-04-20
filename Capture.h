#pragma once
#include <opencv2/opencv.hpp>
#include <mutex>
#include <chrono>
#include "Frame.h"

using namespace cv;
using namespace std;
using namespace chrono;

class Capture
{
	const int minContLenght = 200;
	VideoCapture capture;
	Mat frame;
	Mat mask;
	Mat fgimg;
	BackgroundSubtractorMOG2 bg;
	vector<vector<Point>> all_contours;
	milliseconds currentTime;

	void sort(vector<vector<Point>>);
	bool isIntersected(Rect, Rect);
	void display();
	void displayTime(Mat);

public:
	Capture(int);
	~Capture();
	bool isOpened();
	Mat getFrame();
	void find(vector<Frame>&, mutex&);

};

