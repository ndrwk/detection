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
	const int MINRECTPERIMETR = 100;
	VideoCapture capture;
	Mat frame;
	Mat mask;
	Mat fgimg;
	BackgroundSubtractorMOG2 bg;
	vector<vector<Point>> allContours;
	vector<vector<Point>> allHulls;
	vector<Rect> allRects;
	milliseconds currentTime;
	const int timeRange = 2000; // in milliseconds


	void display();
	void displayTime(Mat);
	void uniteRect(vector<Rect>);


public:
	Capture(int);
	Capture(string);
	~Capture();
	bool isOpened();
	Mat getFrame();
	void find(vector<Frame>&, mutex&);
	void cut(vector<Frame>&, mutex&);

};

