#pragma once
#include <opencv2/opencv.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <mutex>
#include <chrono>
#include "Frame.h"

using namespace cv;
using namespace std;
using namespace chrono;

class Capture
{
	const int MINRECTPERIMETR = 50;
	const int MAXCONTS = 500;
	const int MAX_POINTS = 500;

	VideoCapture capture;
	Mat frame;
	Mat mask;
	Mat fgimg;
	vector<vector<Point>> allContours;
	vector<vector<Point>> allHulls;
	vector<Rect> allRects;
	milliseconds currentTime;
	const int timeRange = 2000; // in milliseconds
	bool initFindPoint;


	void display();
	void displayTime(Mat);
	vector<Rect> uniteRect(vector<vector<Point>>);


public:
	Capture(int);
	Capture(string);
	~Capture();
	bool isOpened();
	Mat getFrame();
	void find(vector<Frame>&, mutex&);
	void cut(vector<Frame>&, mutex&);

};

