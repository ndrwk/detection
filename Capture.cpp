#include "Capture.h"
#include <iostream>
#include <mutex>
#include <ctime>
#include "ContStruct.h"
#include <map>

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


void Capture::cut(map<milliseconds, Frame>& framesFlow, mutex& mutex_frames, vector<map<milliseconds, vector<Point>>>& allTracks, mutex& mutex_tracks)
{
	while (true)
	{
//		cout << "framesFlow.size() = " << framesFlow.size() << endl;
//		cout << "allTracks.size() = " << allTracks.size() << endl;
		milliseconds thisTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		mutex_frames.lock();
		mutex_tracks.lock();

		for (auto frameIt = framesFlow.begin(); frameIt != framesFlow.end();)
		{
			milliseconds frameTime = frameIt->first;
			if ((thisTime.count() - frameTime.count()) > timeRange)
			{
				for (auto trackIt = allTracks.begin(); trackIt != allTracks.end(); )
				{
					if ((trackIt->rbegin()->first != lastTime) & (trackIt->size() < 10))
					{
						trackIt = allTracks.erase(trackIt);
						continue;
					}

					auto mapIt = trackIt->find(frameTime);
					if (mapIt != trackIt->end())
					{
						mapIt = trackIt->erase(mapIt);
					}
					if (trackIt->size() == 0)
					{
						trackIt = allTracks.erase(trackIt);
					}
					else trackIt++;
				}
				frameIt = framesFlow.erase(frameIt);
			}
			else frameIt++;
		}
		mutex_frames.unlock();
		mutex_tracks.unlock();
	}
}


void Capture::find(map<milliseconds, Frame>& framesFlow, mutex& mutex_frames, vector<map<milliseconds, vector<Point>>>& allTracks, mutex& mutex_tracks)
{
	BackgroundSubtractorMOG2 backgroundSubtractor(10, 25, false);
	vector<Vec4i> hierarchy;
	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	Size subPixWinSize(10, 10), winSize(31, 31);
	bool firstTime = true;
	Mat savemask, gray, prevGray;

	while (true)
	{
		Mat frame, mask, fgimg;
		currentTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		capture >> frame;

		backgroundSubtractor(frame, mask, -1);
		Frame frametoMap(frame, mask);

		mutex_frames.lock();
		framesFlow.emplace(currentTime, frametoMap);
		mutex_frames.unlock();
		fgimg = Scalar::all(0);
		frame.copyTo(fgimg, mask);
		mask.copyTo(savemask);
		vector<vector<Point>> allContours;
		findContours(mask, allContours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		if (allContours.size() > 0 && allContours.size() < 1000)
		{
			frame.copyTo(frame);
			cvtColor(frame, gray, COLOR_BGR2GRAY);
			vector<vector<Point>> conts = uniteContours(allContours);
			if (prevGray.empty())
			{
				gray.copyTo(prevGray);
				lastTime = currentTime;
			}

			mutex_tracks.lock();
			if (allTracks.empty())
			{
				for (auto trackIt = conts.begin(); trackIt != conts.end(); trackIt++)
				{
					map<milliseconds, vector<Point>> oneTrack;
					oneTrack.emplace(currentTime, *trackIt);
					allTracks.push_back(oneTrack);
				}
			}
			else
			{
				vector<uchar> status;
				vector<float> err;
				for (auto allTrackIt = allTracks.begin(); allTrackIt != allTracks.end(); allTrackIt++)
				{
					vector<Point2f> pointsPrev, pointsNow;
					auto mapIt = allTrackIt->find(lastTime);
					if (mapIt == allTrackIt->end()) continue;
					pointsPrev = getFeaturePoints(mapIt->second);
					calcOpticalFlowPyrLK(prevGray, gray, pointsPrev, pointsNow, status, err, winSize, 3, termcrit, 0, 0.001);
					for (auto contIt = conts.begin(); contIt != conts.end(); )
					{
						if ((boundingRect(*contIt)&boundingRect(pointsNow)).width > 0)
						{
							allTrackIt->emplace(currentTime, *contIt);
							contIt = conts.erase(contIt);
						} 
						else contIt++;
					}
				}

				for (auto trackIt = conts.begin(); trackIt != conts.end(); trackIt++)
				{
					map<milliseconds, vector<Point>> oneTrack;
					oneTrack.emplace(currentTime, *trackIt);
					allTracks.push_back(oneTrack);
				}
			}
			mutex_tracks.unlock();
//			display(frame, conts);
		}
		int delay = 1;
		waitKey(delay);
		milliseconds endtime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		cout << "cycle time - " << (endtime - currentTime).count() << endl;
		prevGray = gray;
		lastTime = currentTime;
	}


}


void Capture::display(map<milliseconds, Frame>& framesFlow, mutex& mutex_frames, vector<map<milliseconds, vector<Point>>>& allTracks, mutex& mutex_tracks)
{
	while (true)
	{
		mutex_frames.lock();
		if (framesFlow.size() == 0)
		{
			mutex_frames.unlock();
			continue;
		}
		auto frameIt = framesFlow.rbegin();
		mutex_frames.unlock();
		milliseconds time = frameIt->first;
		Mat outFrame;
		outFrame = frameIt->second.getImg();
		mutex_tracks.lock();
		for (auto trackIt = allTracks.begin(); trackIt != allTracks.end(); trackIt++)
		{
			auto mapIt = trackIt->find(time);
			if (mapIt==trackIt->end()) continue;
			int number = trackIt - allTracks.begin();
			rectangle(outFrame, boundingRect(mapIt->second), Scalar(255, 0, 0), 1, 8, 0);
			stringstream ss;
			ss << number;
			string stringNumber = ss.str();
			putText(outFrame, stringNumber, Point(boundingRect(mapIt->second).x + 5, boundingRect(mapIt->second).y + 5),
				FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar::all(255), 1, 8);
		}
		mutex_tracks.unlock();
		imshow("frame", outFrame);
		waitKey(10);
	}
}



/*
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
*/

void Capture::displayTime(Mat img)
{
	seconds sec = duration_cast<seconds>(currentTime);
	time_t timefordisp = sec.count();
	putText(img, ctime(&timefordisp), Point(20, img.rows-40),FONT_HERSHEY_COMPLEX,1,Scalar::all(255),1,8);
}
