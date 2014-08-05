#include "Capture.h"
#include <iostream>
#include <mutex>
#include <ctime>
using namespace std;
using namespace chrono;


Capture::Capture(int cameraNumber)
{
	capture.open(cameraNumber);
	initFindPoint = true;
}

Capture::Capture(string fileName)
{
	capture.open(fileName);
	initFindPoint = true;
}

Capture::~Capture()
{

}

bool Capture::isOpened()
{
	return capture.isOpened();
}


Mat Capture::getFrame()
{
	Mat m;
	return m;
}



vector<Rect> Capture::uniteRect(vector<vector<Point>> contours)
{
	vector<Rect> rects;
	for (auto i = contours.begin(); i != contours.end(); i++)
	{
		rects.push_back(boundingRect(*i));
	}
	bool isCrossed = true;
	Rect stub(1, 1, 1, 1);
	while (isCrossed)
	{
		isCrossed = false;
		for (auto i = rects.begin(); i != rects.end();)
		{
			if (((*i).height + (*i).width) < MINRECTPERIMETR)
			{
				i = rects.erase(i);
			}
			else
			{
				for (auto j = i; j != rects.end(); j++)
				{
					if (i == j) continue;
					if ((*i & *j).width != 0)
					{
						*i = *i | *j;
						*j = stub;
						isCrossed = true;
					}
				}
				i++;
			}
		}
	}
	return rects;
}

void Capture::cut(vector<Frame>& frames, mutex& mutex_frames)
{
	while (true)
	{
		mutex_frames.lock();
		for (auto iter = frames.begin(); iter != frames.end();)
		{
			Frame frame = *iter;
			milliseconds timeNow = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
			if ((timeNow.count() - frame.getTime()) > timeRange)
			{
				iter = frames.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		mutex_frames.unlock();
	}
}


void Capture::find(vector<Frame>& frames, mutex& mutex_frames)
{
	milliseconds lastTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
	BackgroundSubtractorMOG2 bg(10, 25, false);
	vector<Vec4i> hierarchy;
	vector<Point2f> pointsPrev, pointsNow;
	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	Size subPixWinSize(10, 10), winSize(31, 31);
	while (true)
	{
		currentTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		Mat frame, mask, fgimg;
		capture >> frame;
		bg(frame, mask, -1);
		fgimg = Scalar::all(0);
		frame.copyTo(fgimg, mask);
		vector<vector<Point>> allContours;
		findContours(mask, allContours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		if ((allContours.size() > 0) && (allContours.size() < MAXCONTS))
		{
			lastTime = currentTime;
			allRects.clear();
			allRects = uniteRect(allContours);
			Frame forSave(currentTime, frame, allRects, fgimg);
			mutex_frames.lock();
			frames.push_back(forSave);
			mutex_frames.unlock();
		}
		waitKey(20);
	}
}


void Capture::display(Mat frame)
{
	for (auto i = allRects.begin(); i != allRects.end(); i++)
	{
		int number = i - allRects.begin();
		stringstream ss;
		ss << number;
		string stringNumber = ss.str();
		rectangle(frame, *i, Scalar(255, 0, 0), 1, 8, 0);
		putText(frame, stringNumber, Point((*i).x + 5, (*i).y + 5), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar::all(255), 1, 8);
	}
	imshow("frame", frame);
}

void Capture::displayTime(Mat img)
{
	seconds sec = duration_cast<seconds>(currentTime);
	time_t timefordisp = sec.count();
	putText(img, ctime(&timefordisp), Point(20, img.rows-40),FONT_HERSHEY_COMPLEX,1,Scalar::all(255),1,8);
}
