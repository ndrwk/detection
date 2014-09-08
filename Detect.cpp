#include "Detect.h"
#include <mutex>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching/stitcher.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <thread>
#include <chrono>
#include "RectStruct.h"

using namespace std;
using namespace cv;

Detect::Detect()
{
}


Detect::~Detect()
{
}





/*
void Detect::detectPoints(vector<Frame>& frames, mutex& mutex_frames)
{
	bool need2Init = true;
	vector<Point2f> pointsNow, pointsPrev;
	//	vector<set<int>> rSets;
	vector<uchar> status;
	vector<float> err;
	//	vector<Rect> rcts;
	vector<RectStruct> rstrNow, rstrPrev;
	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	Size subPixWinSize(10, 10), winSize(31, 31);

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
			rstrNow.clear();
			if (need2Init)
			{
				for (auto r = rectsPrev.begin(); r != rectsPrev.end(); r++)
				{
					Rect rect = *r;
					int number = r - rectsPrev.begin();
					RectStruct rs(number, rect);
					rstrNow.push_back(rs);
				}
				cout << "init  " << rstrNow.size() << endl;
				need2Init = false;
			}
			else
			{
				if (!rstrPrev.empty())
				{
					for (auto i = rstrPrev.begin(); i != rstrPrev.end();i++)
					{
						Rect r = (*i).getRect();
						int number = (*i).getNumber();
						vector<Point2f> reperPointsPrev, reperPointsNow;
						reperPointsPrev.push_back(Point2f(r.x, r.y));
						reperPointsPrev.push_back(Point2f(r.x + r.width, r.y + r.height));
						calcOpticalFlowPyrLK(grayPrev, grayNow, reperPointsPrev, reperPointsNow, status, err, winSize, 3, termcrit, 0, 0.001);
						Rect rect(reperPointsNow[0], reperPointsNow[1]);
						for (auto j = rectsNow.begin(); j != rectsNow.end();  )
						{
							Rect rtmp = *j;
							if ((rtmp & rect).width == rect.width)
							{
								cout << "number-" << number << endl;
								rstrNow.push_back(RectStruct(number, rect));
								j = rectsNow.erase(j);
							}
							else
							{
								j++;
							}
						}
						int ofset = rstrNow.size();
						cout << "ofset   " << ofset << endl;

						for (auto ir = rectsNow.begin(); ir != rectsNow.end(); ir++)
						{
							int num = ir - rectsNow.begin();
							cout << "number+" << number << endl;
							Rect rct = *ir;
//							rstrNow.push_back(RectStruct(ofset + num, rct));
						}


					}
					for (auto i = rstrNow.begin(); i != rstrNow.end(); i++)
					{
						stringstream ss;
						ss << (*i).getNumber();
						string stringNumber = ss.str();
						cout << stringNumber << endl;
						putText(imgNow, stringNumber, Point((*i).getRect().x + 5, (*i).getRect().y + 5), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar::all(255), 1, 8);
						rectangle(imgNow, (*i).getRect(), Scalar(255, 0, 0));
					}


				}
				else
					need2Init = true;



				imshow("points", imgNow);
			}
			swap(rstrNow, rstrPrev);

		}
		waitKey(20);
	}
}
*/


/*
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
*/


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
