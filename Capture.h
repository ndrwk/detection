#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <mutex>
#include <chrono>
#include "Frame.h"
#include <vector>

using namespace cv;
using namespace std;
using namespace chrono;

class Capture
{
	const int MINRECTPERIMETR = 50;

	VideoCapture capture;
	milliseconds currentTime, lastTime;
	const int timeRange = 3000; // in milliseconds
	int fps;


	vector <Point2f> getFeaturePoints(vector<Point>);
	void displayTime(Mat);
	vector<Rect> uniteRect(vector<vector<Point>>);
	vector<vector<Point>> uniteContours(vector<vector<Point>>);
	vector<Point> convertBack(vector<Point2f>);
	vector <Point2f> getPoints(Rect);

public:
	Capture(int);
	Capture(string);
	~Capture();
	bool isOpened();
	Mat getFrame();
	void find(map<milliseconds, Frame>&, mutex&, vector<map<milliseconds, vector<Point>>>&, mutex&);
	void cut(map<milliseconds, Frame>&, mutex&, vector<map<milliseconds, vector<Point>>>&, mutex&);
	void display(map<milliseconds, Frame>&, mutex&, vector<map<milliseconds, vector<Point>>>&, mutex&);

};
