#include "Capture.h"
#include <iostream>
#include <mutex>
#include <ctime>
using namespace std;
using namespace chrono;


Capture::Capture(int cameraNumber)
{
	cap.open(cameraNumber);
}

Capture::~Capture()
{

}

bool Capture::isOpened()
{
	return cap.isOpened();
}


Mat Capture::getFrame()
{
	return frame;
}

void Capture::getFound(vector<Square>& squares, mutex& mutex_squares)
{
	while (true)
	{
		currentTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		cap >> frame;
		bg(frame, mask, 0.01);
		erode(mask, mask, Mat());
		dilate(mask, mask, Mat());
		fgimg = Scalar::all(0);
		frame.copyTo(fgimg, mask);
		findContours(mask, all_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		for (vector<vector<Point>>::iterator iter = all_contours.begin(); iter != all_contours.end();)
		{
			vector <Point> contour;
			contour = *iter;
			if (contour.size() < 200)
			{
				iter = all_contours.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		vector <Rect> bounds;
		if (all_contours.size() > 0)
		{
			Rect bound_rect = boundingRect(all_contours[0]);
			for (vector<vector<Point>>::iterator iter = all_contours.begin(); iter != all_contours.end();)
			{
				if (isIntersected(bound_rect, boundingRect(*iter)))
				{
					bound_rect = bound_rect | boundingRect(*iter);
					iter = all_contours.erase(iter);
				}
				else
				{
					++iter;
				}
			}
			for (vector<vector<Point>>::iterator iter = all_contours.begin(); iter != all_contours.end();)
			{
				if (isIntersected(bound_rect, boundingRect(*iter)))
				{
					bound_rect = bound_rect | boundingRect(*iter);
					iter = all_contours.erase(iter);
				}
				else
				{
					bounds.push_back(boundingRect(*iter));
					++iter;
				}
			}
			bounds.push_back(bound_rect);
			for (vector<Rect>::iterator iter = bounds.begin(); iter != bounds.end(); ++iter)
			{
				int side;
				Rect rect = *iter;
				if (rect.height > rect.width){
					side = rect.height;
				}
				else {
					side = rect.width;
				}
				if (((rect.x + side) < frame.cols) & ((rect.y + side) < frame.rows)) {
					Rect squareBond(rect.x, rect.y, side, side);
					rectangle(mask, squareBond, Scalar(255, 255, 255), 1);
					Mat croppedImage = fgimg(squareBond);
//					Mat croppedImage = frame(squareBond);
					Square square(currentTime, croppedImage);
					mutex_squares.lock();
					squares.push_back(square);
					mutex_squares.unlock();
				}
			}
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
