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

void Capture::sort(vector<vector<Point>> contours)
{
	vector<Point> temp;
	for (int i = 0; i < (contours.size() - 1); i++)
	{
		for (int j = 0; j < contours.size() - i - 1; j++)
		{
			if (contours[j].size() < contours[j + 1].size())
			{
				temp = contours[j];
				contours[j] = contours[j + 1];
				contours[j + 1] = temp;
			}
		}
	}
}

void Capture::find(vector<Frame>& frames, mutex& mutex_frames)
{
	while (true)
	{
		currentTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		capture >> frame;
		bg(frame, mask, 0.01);
		erode(mask, mask, Mat());
		dilate(mask, mask, Mat());
		fgimg = Scalar::all(0);
		frame.copyTo(fgimg, mask);
		findContours(mask, all_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		for (vector<vector<Point>>::iterator iter = all_contours.begin(); iter != all_contours.end();)
		{
			vector <Point> contour = *iter;
			if (contour.size() < minContLenght)
			{
				iter = all_contours.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		if (all_contours.size() > 0)
		{
			sort(all_contours);
			Frame forSave(currentTime, frame, all_contours);
			mutex_frames.lock();
			frames.push_back(forSave);
			mutex_frames.unlock();
		}
		displayTime(frame);
		display();
		waitKey(20);
	}
}

bool Capture::isIntersected(Rect rect1, Rect rect2)
{
	return ((rect1 & rect2).width != 0);
}

void Capture::display()
{
	drawContours(mask, all_contours, -1, Scalar(255, 0, 0), 2);
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
