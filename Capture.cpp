#include "Capture.h"
#include <iostream>
#include <mutex>
#include <ctime>
using namespace std;
using namespace chrono;


Capture::Capture(int cameraNumber)
{
	capture.open(cameraNumber);
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
	return frame;
}


/*
void Capture::sort(vector<Rect> rects)
{
	Rect temp;
	for (int i = 0; i < (rects.size() - 1); i++)
	{
		for (int j = 0; j < rects.size() - i - 1; j++)
		{
			if ((rects[j].height+rects[j].width)<(rects[j+1].height+rects[j+1].width))
			{
				temp = rects[j];
				rects[j] = rects[j + 1];
				rects[j + 1] = temp;
			}
		}
	}
}
*/


void Capture::uniteRect(vector<Rect> rects)
{
	Rect stub(1, 1, 1, 1);
	for (vector<Rect>::iterator i = rects.begin(); i != rects.end(); i++)
	{
		for (vector<Rect>::iterator j = rects.begin(); j != rects.end(); j++)
		{
			if ((((*i & *j).width != 0)) && (i-rects.begin() != j-rects.begin()))
			{
				*i = *i | *j;
				*j = stub;
			}
		}
	}
	for (vector<Rect>::iterator iter = rects.begin(); iter != rects.end();)
	{
		Rect tmp = *iter;
		if ((tmp.height == 1)&(tmp.width == 1))
		{
			iter = rects.erase(iter);
		}
		else
		{
			iter++;
		}
	}

}

void Capture::cut(vector<Frame>& frames, mutex& mutex_frames)
{
	while (true)
	{
		mutex_frames.lock();
		for (vector<Frame>::iterator iter = frames.begin(); iter != frames.end();)
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
	while (true)
	{
		currentTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		capture >> frame;
		bg(frame, mask, 0.01);
//		erode(mask, mask, Mat());
//		dilate(mask, mask, Mat());
		Mat temp;
		const int niters = 3;
		dilate(mask, temp, Mat(), Point(-1, -1), niters);
		erode(temp, temp, Mat(), Point(-1, -1), niters * 2);
		dilate(temp, temp, Mat(), Point(-1, -1), niters);

		fgimg = Scalar::all(0);
		frame.copyTo(fgimg, mask);
		findContours(temp, allContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		if ((allContours.size() > 0)&(allContours.size()<200))
		{
			vector<Rect> rects;
			for (vector<vector<Point>>::iterator contIter0 = allContours.begin(); contIter0 != allContours.end(); )
			{
				vector<Point> contour = *contIter0;
				if (contour.size() < MINRECTPERIMETR)
				{
					contIter0 = allContours.erase(contIter0);
				}
				else
				{
					Rect rect = boundingRect(contour);
					rects.push_back(rect);
					//				rectangle(frame, rect, Scalar(0, 0, 255), 1, 8, 0);
					contIter0++;
				}
			}
			uniteRect(rects);
			for (vector<Rect>::iterator iter = rects.begin(); iter != rects.end();)
			{
				Rect tmpRect = *iter;
				if (2*(tmpRect.width+tmpRect.height) < MINRECTPERIMETR)
				{
					iter = rects.erase(iter);
				}
				else
				{
					++iter;
				}
			}
			allRects = rects;
			Frame forSave(currentTime, frame, allRects, fgimg, allContours);
			mutex_frames.lock();
			frames.push_back(forSave);
			mutex_frames.unlock();
		}
//		displayTime(frame);
		display();
		waitKey(20);
	}
}


void Capture::display()
{
	for (vector<Rect>::iterator i = allRects.begin(); i != allRects.end(); i++)
	{
		int number = i - allRects.begin();
		stringstream ss;
		string stringNumber;
		ss << number;
		stringNumber = ss.str();
		rectangle(fgimg, *i, Scalar(255, 0, 0), 1, 8, 0);
		putText(fgimg, stringNumber, Point((*i).x + 5, (*i).y + 5), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar::all(255), 1, 8);
	}
	drawContours(mask, allContours, -1, Scalar(255, 0, 0), 2);
	imshow("mask", mask);
	imshow("fgimg", fgimg);
	imshow("frame", frame);
}

void Capture::displayTime(Mat img)
{
	seconds sec = duration_cast<seconds>(currentTime);
	time_t timefordisp = sec.count();
	putText(img, ctime(&timefordisp), Point(20, img.rows-40),FONT_HERSHEY_COMPLEX,1,Scalar::all(255),1,8);
}
