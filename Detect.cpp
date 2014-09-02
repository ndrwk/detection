#include "Detect.h"
#include <mutex>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching/stitcher.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <thread>
#include <chrono>
#include "Vortex.h"
#include "RectStruct.h"

using namespace std;
using namespace cv;

Detect::Detect()
{
}


Detect::~Detect()
{
}



void Detect::d_etectPoints(vector<Frame>& frames, mutex& mutex_frames)
{
	bool need2Init = true;
	vector<Point2f> pointsNow, pointsPrev;
//	vector<set<int>> rSets;
	vector<uchar> status;
	vector<float> err;
//	vector<Rect> rcts;
	vector<RectStruct> rstrctsNow, rstrctsPrev;
	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	Size subPixWinSize(10, 10), winSize(31, 31);
	int ofset = 0;

	while (true)
	{
		if (frames.size() > 6)
		{
			mutex_frames.lock();
			auto it = frames.end() - 1;
			vector<Rect> rectsNow = (*it).getRects();
			Mat imgNow = (*it).getFgimg();
			it--;
			Mat imgPrev = (*it).getFgimg();
			vector<Rect> rectsPrev = (*it).getRects();
			mutex_frames.unlock();
			Mat grayNow, grayPrev;
			cvtColor(imgNow, grayNow, COLOR_BGR2GRAY);
			cvtColor(imgPrev, grayPrev, COLOR_BGR2GRAY);
			rstrctsNow.clear();

			if (need2Init)
			{
				for (auto r = rectsNow.begin(); r != rectsNow.end(); r++)
				{
					Rect rect = *r;
					vector<Point2f> points;
					int n = r - rectsNow.begin();
					int num = ofset + n;
					Mat mask(grayNow.size(), CV_8UC1);
					mask.setTo(Scalar::all(0));
					rectangle(mask, rect, Scalar::all(255), -1);
					goodFeaturesToTrack(grayNow, points, MAXCORNERS, 0.01, 10, mask, 3, 0, 0.04);
					RectStruct rs(num, rect, points);
					rstrctsNow.push_back(rs);
				}
				cout << "init" << endl;
				need2Init = false;

			}
			else
			{
				int count = 0;
				for (auto r = rstrctsPrev.begin(); r != rstrctsPrev.end(); r++)
				{
					vector<Point2f> pPrev = (*r).getSetOfPoints();
					vector<Point2f> pNow;
					int number = (*r).getNumber();
					calcOpticalFlowPyrLK(grayPrev, grayNow, pPrev, pNow, status, err, winSize, 3, termcrit, 0, 0.001);
					int xMin = grayNow.cols;
					int xMax = 0;
					int yMin = grayNow.rows;
					int yMax = 0;
					for (auto s = pNow.begin(); s != pNow.end(); s++)
					{
						Point2f point = *s;
						if (point.x < xMin) xMin = (int)point.x;
						if (point.x > xMax) xMax = (int)point.x;
						if (point.y < yMin) yMin = (int)point.y;
						if (point.y > yMax) yMax = (int)point.y;
					}

					Rect rect(Point(xMin, yMin), Point(xMax, yMax));
					Rect rtmp;

					for (auto rn = rectsNow.begin(); rn != rectsNow.end(); rn++)
					{
						if ((rect & *rn).width != 0) rtmp = *rn;
						else count++;
					}

					RectStruct rs(number, rtmp, pNow);
					rstrctsNow.push_back(rs);

					//						rcts.push_back(rect);
					rectangle(imgNow, rect, Scalar(0, 0, 255));
					stringstream ss;
					ss << number;
					string stringNumber = ss.str();
					putText(imgNow, stringNumber, Point(rect.x + 5, rect.y + 5), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar::all(255), 1, 8);
				}
//				if (count > 0) need2Init = true;




				/*
									for (int i = 0; i < pointsNow.size(); i++)
									{
									line(imgNow, pointsPrev[i], pointsNow[i], Scalar(255, 255, 0));
									}
									*/


			imshow("points", imgNow);
			}
			
			swap(pointsNow, pointsPrev);
			swap(rstrctsNow, rstrctsPrev);
//			swap(rSetsNow, rSetsPrev);
		}
		waitKey(20);
	}
}


void Detect::detectPoints(vector<Frame>& frames, mutex& mutex_frames)
{
	bool need2Init = true;
	vector<Point2f> pointsNow, pointsPrev;
	//	vector<set<int>> rSets;
	vector<uchar> status;
	vector<float> err;
	//	vector<Rect> rcts;
	vector<RectStruct> rstrctsNow, rstrctsPrev;
	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	Size subPixWinSize(10, 10), winSize(31, 31);
	int ofset = 0;

	while (true)
	{
		if (frames.size() > 6)
		{
			mutex_frames.lock();
			auto it = frames.end() - 1;
			vector<Rect> rectsNow = (*it).getRects();
			Mat imgNow = (*it).getFgimg();
			it--;
			Mat imgPrev = (*it).getFgimg();
			vector<Rect> rectsPrev = (*it).getRects();
			mutex_frames.unlock();
			Mat grayNow, grayPrev;
			cvtColor(imgNow, grayNow, COLOR_BGR2GRAY);
			cvtColor(imgPrev, grayPrev, COLOR_BGR2GRAY);
			rstrctsNow.clear();
			vector<Point2f> reperPointsPrev, reperPointsNow;
			for (auto i = rectsPrev.begin(); i != rectsPrev.end(); i++)
			{
				Rect r = *i;
				reperPointsPrev.push_back(Point2f(r.x + 2, r.y + 2));
				reperPointsPrev.push_back(Point2f(r.x + r.width - 2, r.y + 2));
				reperPointsPrev.push_back(Point2f(r.x + r.width - 2, r.y + r.height - 2));
				reperPointsPrev.push_back(Point2f(r.x + 2, r.y + r.height - 2));
			}
			calcOpticalFlowPyrLK(grayPrev, grayNow, reperPointsPrev, reperPointsNow, status, err, winSize, 3, termcrit, 0, 0.001);



			for (auto i = reperPointsPrev.begin(); i != reperPointsPrev.end(); i++)
			{
				circle(imgNow, *i, 2, Scalar(255, 0, 0), -1);
			}
			for (auto i = reperPointsNow.begin(); i != reperPointsNow.end(); i++)
			{
				circle(imgNow, *i, 2, Scalar(0, 255, 0), -1);
			}
			for (auto i = rectsNow.begin(); i != rectsNow.end(); i++)
			{

				rectangle(imgNow, *i, Scalar(255, 0, 0));
			}
			imshow("points", imgNow);
			

		}
		waitKey(20);
	}
}


void Detect::display(vector<Frame>& frames, mutex& mutex_frames)
{
	while (true)
	{
		if (!frames.empty())
		{
			mutex_frames.lock();
			auto iter = frames.end()-1;
			vector<Rect> rects = (*iter).getRects();
			Mat img = (*iter).getImg();
			long long time = (*iter).getTime();
			mutex_frames.unlock();
			for (auto r = rects.begin(); r != rects.end(); r++)
			{
				int number = r - rects.begin();
				stringstream ss;
				ss << number;
				string stringNumber = ss.str();
				rectangle(img, *r, Scalar(255, 0, 0), 1, 8, 0);
				putText(img, stringNumber, Point((*r).x + 5, (*r).y + 5), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar::all(255), 1, 8);
			}
			//			putText(img, ctime(&time), Point(20, img.rows - 40), FONT_HERSHEY_COMPLEX, 1, Scalar::all(255), 1, 8);

			imshow("img", img);
			waitKey(20);
		}
	}





}


/*
sort(vortexes.begin(), vortexes.end(), Vortex::cmpByAngle);
vector<vector<Vortex>> fragments;
vector<Vortex> region;

auto i = vortexes.begin();
region.push_back(*i);
i++;
for (; i != vortexes.end(); i++)
{
	if ((*i).getAngle() == (*(i - 1)).getAngle())
	{
		region.push_back(*i);
	}
	else
	{
		fragments.push_back(region);
		region.clear();
		region.push_back(*i);
	}
}
fragments.push_back(region);
const Scalar colors[13] = { BLACK, WHITE, RED, ORANGE, YELLOW, GREEN, LIGHTBLUE, BLUE, VIOLET, GINGER, PINK, LIGHTGREEN, BROWN };

int c = 0;
for (auto i = fragments.begin(); i != fragments.end(); i++)
{
	vector<Vortex> reg = *i;
	for (auto j = reg.begin(); j != reg.end(); j++)
	{
		Point2f point = (*j).getP1();
		circle(imgNow, point, 3, colors[c], -1);
	}
	c++;
}
*/
