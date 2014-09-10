#include "Capture.h"
#include <iostream>
#include <mutex>
#include <ctime>
#include "ContStruct.h"

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

vector<vector<Point>> Capture::uniteContours(vector<vector<Point>> cnts)
{
	vector<vector<Point>> contours = cnts;
	bool isCrossed = true;
	vector<Point> stub = { Point(1, 1) };
	while (isCrossed)
	{
		isCrossed = false;
		for (auto i = contours.begin(); i != contours.end();)
		{
			if ((*i).size() < 30)
			{
				i = contours.erase(i);
			}
			else
			{
				for (auto j = i; j != contours.end(); j++)
				{
					if (i == j) continue;
					if ((boundingRect(*i) & boundingRect(*j)).width != 0)
					{
						(*i).insert((*i).end(), (*j).begin(), (*j).end());
						*j = stub;
						isCrossed = true;	
					}
				}
				i++;
			}
		}
	}
	return contours;
}


vector <Point2f> Capture::getFeaturePoints(vector<Point> contours)
{
	vector<Point2f> features;
	features.push_back(Point2f((float)contours[0].x, (float)contours[0].y));
	features.push_back(Point2f((float)contours[(int)contours.size() / 2].x, (float)contours[(int)contours.size() / 2].y));
	features.push_back(Point2f((float)contours[contours.size() - 1].x, (float)contours[contours.size() - 1].y));
	return features;
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
//	milliseconds lastTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
	BackgroundSubtractorMOG2 bg(10, 25, false);
	vector<Vec4i> hierarchy;
	vector<Point2f> pointsPrev, pointsNow;
	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	Size subPixWinSize(10, 10), winSize(31, 31);
	bool firstTime = true;
	Mat frame, mask, fgimg, savemask, gray, prevGray;
	vector<ContStruct> cstructs[2];

	while (true)
	{
		currentTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		capture >> frame;
		bg(frame, mask, -1);
		fgimg = Scalar::all(0);
		frame.copyTo(fgimg, mask);
		mask.copyTo(savemask);
		vector<vector<Point>> allContours;
		vector<Rect> tmpRects;
		findContours(mask, allContours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		frame.copyTo(frame);
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		allContours = uniteContours(allContours);
		vector<vector<Point>> conts = allContours;
		if (cstructs[0].empty())
		{
			for (auto i = conts.begin(); i != conts.end(); i++)
			{
				int num = (int)(i - conts.begin());

				vector <Point2f> features = getFeaturePoints(*i);
			}

		}
		display(frame, allContours);
		int delay = 1;
		waitKey(delay);
		milliseconds endtime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		cout << "cycle time - " << (endtime - currentTime).count() << endl;
		swap(prevGray, gray);
	}


}


void Capture::display(Mat frame, vector<vector<Point>> conts)
{
	drawContours(frame, conts, -10, Scalar::all(255));
	for (auto i = conts.begin(); i != conts.end(); i++)
	{
		vector <Point2f> features = getFeaturePoints(*i);
		for (auto it = features.begin(); it != features.end(); it++)
		{
			circle(frame, *it, 2, Scalar(0, 0, 255), -1);
		}
		int number = i - conts.begin();
		stringstream ss;
		ss << number;
		string stringNumber = ss.str();
		rectangle(frame, boundingRect(*i), Scalar(255, 0, 0), 1, 8, 0);
		putText(frame, stringNumber, Point(boundingRect(*i).x + 5, boundingRect(*i).y + 5), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar::all(255), 1, 8);
	}
	imshow("frame", frame);
}

void Capture::displayTime(Mat img)
{
	seconds sec = duration_cast<seconds>(currentTime);
	time_t timefordisp = sec.count();
	putText(img, ctime(&timefordisp), Point(20, img.rows-40),FONT_HERSHEY_COMPLEX,1,Scalar::all(255),1,8);
}
