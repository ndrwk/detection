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
//	milliseconds lastTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
	BackgroundSubtractorMOG2 bg(10, 25, false);
	vector<Vec4i> hierarchy;
	vector<Point2f> pointsPrev, pointsNow;
	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	Size subPixWinSize(10, 10), winSize(31, 31);
	bool firstTime = true;
	vector<Point2f> points[2];
	vector<Rect> rects[2];
	Mat frame, mask, fgimg, savemask, gray, prevGray;


	while (true)
	{
		currentTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		capture >> frame;
		bg(frame, mask, -1);
		fgimg = Scalar::all(0);
		frame.copyTo(fgimg, mask);
		mask.copyTo(savemask);
		vector<vector<Point>> allContours;
		findContours(mask, allContours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		if ((allContours.size() > 0) && (allContours.size() < MAXCONTS))
		{
			frame.copyTo(frame);
			cvtColor(frame, gray, COLOR_BGR2GRAY);
//			lastTime = currentTime;
			vector<Rect> tmpRects = uniteRect(allContours);
			cout << tmpRects.size() << "  ";
			rects[1].clear();
			Frame forSave(currentTime, frame, savemask);
			if (firstTime)
			{
				goodFeaturesToTrack(gray, points[1], 500, 0.01, 10, Mat(), 3, 0, 0.04);
				cornerSubPix(gray, points[1], subPixWinSize, Size(-1, -1), termcrit);
				firstTime = false;
				rects[1] = tmpRects;
				swap(rects[1], rects[0]);
				swap(points[1], points[0]);
				swap(prevGray, gray);
				continue;
			}
			if (!points[0].empty())
			{
				vector<uchar> status;
				vector<float> err;
				if (prevGray.empty()) gray.copyTo(prevGray);
				calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.001);
				for (auto rectIter0 = rects[0].begin(); rectIter0 != rects[0].end(); rectIter0++)
				{
					Rect rct1 = *rectIter0;
					int number = rectIter0 - rects[0].begin();
					for (auto pointIter0 = points[0].begin(); pointIter0 != points[0].end(); pointIter0++)
					{
						Rect rct0 = *rectIter0;
						int ofset = pointIter0 - points[0].begin();
						auto pointIter1 = points[1].begin() + ofset;
						Point2f pnt1 = *pointIter1;
						Point2f pnt0 = *pointIter0;
						bool breakFlag = false;
						if ((pnt0.x >= rct0.x) && (pnt0.x <= (rct0.x + rct0.width)) && (pnt0.y >= rct0.y) && (pnt0.y <= (rct0.y + rct0.height)))
						{
							for (auto rectIter1 = tmpRects.begin(); rectIter1 != tmpRects.end(); )
							{
								Rect rct1 = *rectIter1;
								if ((pnt1.x >= rct1.x) && (pnt1.x <= (rct1.x + rct1.width)) && (pnt1.y >= rct1.y) && (pnt1.y <= (rct1.y + rct1.height)))
								{
									rects[1].push_back(rct1);
									rectIter1 = tmpRects.erase(rectIter1);
									breakFlag = true;
									break;
								}
								else
								{
									rectIter1++;
								}
							}
						}
						if (breakFlag) break;
					}
				}
				cout << tmpRects.size() << endl;
				for (auto rectIter1 = tmpRects.begin(); rectIter1 != tmpRects.end(); rectIter1++)
				{
					Rect rct1 = *rectIter1;
					rects[1].push_back(rct1);
					Mat roi(gray.size(), CV_8UC1);
					roi.setTo(Scalar::all(0));
					rectangle(roi, rct1, Scalar::all(255), -1);
					vector<Point2f> roiPoints;
					goodFeaturesToTrack(gray, roiPoints, 10, 0.01, 10, roi, 3, 0, 0.04);
//					cornerSubPix(gray, points[1], subPixWinSize, Size(-1, -1), termcrit);
					points[1].insert(points[1].end(), roiPoints.begin(), roiPoints.end());
				}

				mutex_frames.lock();
				frames.push_back(forSave);
				mutex_frames.unlock();
				display(frame, rects[1]);
			}
		}
		int delay = 1;
		waitKey(delay);
		swap(rects[1], rects[0]);
		swap(points[1], points[0]);
		swap(prevGray, gray);
	}
}


void Capture::display(Mat frame, vector<Rect> allRects)
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
