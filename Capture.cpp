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

Capture::Capture(string fileName)
{
	capture.open(fileName);
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
	while (true)
	{
		currentTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		capture >> frame;


/*
		Mat tmp, tmp1, tmp2;
		int morph_size = 1;
		int size = 2 * morph_size + 1;
		Mat element = getStructuringElement(MORPH_CROSS, Size(size, size), Point(-1,-1));
		morphologyEx(frame, tmp, MORPH_GRADIENT, element);
		cvtColor(tmp, tmp, CV_RGB2GRAY);
		imshow("1", tmp);
		threshold(tmp, tmp1, 20, 255, THRESH_BINARY);
		imshow("2", tmp1);
*/

		bg(frame, mask, 0.01);
		Mat temp;

		const int niters = 3;
		dilate(mask, temp, Mat(), Point(-1, -1), niters);
		erode(temp, temp, Mat(), Point(-1, -1), niters * 2);
		dilate(temp, temp, Mat(), Point(-1, -1), niters);
		fgimg = Scalar::all(0);
		frame.copyTo(fgimg, mask);
		allHulls.clear();
		allContours.clear();
		findContours(temp, allContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

/*
		allHulls.clear();
		findContours(tmp1, allContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
*/
		if ((allContours.size() > 0)&(allContours.size()<2000))
		{
			vector<Rect> rects;
			for (auto contIter0 = allContours.begin(); contIter0 != allContours.end(); )
			{
				vector<Point> contour = *contIter0;
				vector<Point> hull;
				if (contour.size() < MINRECTPERIMETR)
				{
					contIter0 = allContours.erase(contIter0);
				}
				else
				{
					convexHull(Mat(contour), hull, false);
					allHulls.push_back(hull);
					Rect rect = boundingRect(hull);
					rects.push_back(rect);
					//				rectangle(frame, rect, Scalar(0, 0, 255), 1, 8, 0);
					contIter0++;
				}
			}
			uniteRect(rects);
			for (auto iter = rects.begin(); iter != rects.end();)
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
/*
	Mat tmp, tmp1, tmp2;
	cvtColor(frame, tmp2, CV_RGB2GRAY);
	int morph_elem = 0;
	int morph_size = 1;
	Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
	morphologyEx(tmp2, tmp, MORPH_GRADIENT, element);
	imshow("morph", tmp);
	vector<vector<Point>> conts;
	threshold(tmp, tmp1, 10, 255, THRESH_BINARY);
//	findContours(tmp1, conts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//	drawContours(tmp1, conts, -1, Scalar(255, 0, 0), 2);
	imshow("123", tmp1);
*/

	for (auto i = allRects.begin(); i != allRects.end(); i++)
	{
		int number = i - allRects.begin();
		stringstream ss;
		ss << number;
		string stringNumber = ss.str();
		rectangle(frame, *i, Scalar(255, 0, 0), 1, 8, 0);
		putText(frame, stringNumber, Point((*i).x + 5, (*i).y + 5), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar::all(255), 1, 8);
	}
	drawContours(frame, allHulls, -1, Scalar(0, 255, 0), 2);
	//	drawContours(frame, allContours, -1, Scalar(255, 0, 0), 2);
//	imshow("mask", mask);
//	imshow("fgimg", fgimg);
	imshow("frame", frame);
}

void Capture::displayTime(Mat img)
{
	seconds sec = duration_cast<seconds>(currentTime);
	time_t timefordisp = sec.count();
	putText(img, ctime(&timefordisp), Point(20, img.rows-40),FONT_HERSHEY_COMPLEX,1,Scalar::all(255),1,8);
}
