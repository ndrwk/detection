#include "Capture.h"

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
	capture.release();

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
//	Rect stub(1, 1, 1, 1);
	while (isCrossed)
	{
		isCrossed = false;
		for (auto i = rects.begin(); i != rects.end();)
		{
			if ((i->height + i->width) < MINRECTPERIMETR)
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
						*j = Rect(1, 1, 1, 1);
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
			if (i->size() < 30)
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
						i->insert(i->end(), j->begin(), j->end());
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
	const int qty = 10;
	long step = (long)contours.size()/qty;
	for (auto i = contours.begin(); i < contours.end(); i+=step)
	{
		features.push_back(Point2f(i->x, i->y));
	}
	return features;
}

//vector <Point> Capture::convertBack(vector<Point2f> features)
//{
//	vector <Point> contour;
//	for (auto i = features.begin(); i != features.end(); i++)
//	{
//		contour.push_back(Point((int)i->x, (int)i->y));
//	}
//	return contour;
//}

//vector <Point2f> Capture::getPoints(Rect rect)
//{
//	vector <Point2f> features;
//	float x = (float)rect.x;
//	float y = (float)rect.y;
//	features.push_back(Point2f(x, y));
//	features.push_back(Point2f(x + rect.width, y));
//	features.push_back(Point2f(x + rect.width, y + rect.height));
//	features.push_back(Point2f(x, y + rect.height));
//	return features;
//}


void Capture::cut(map<milliseconds, Frame>& framesFlow, mutex& mutex_frames, vector<map<milliseconds, vector<Point>>>& allTracks, mutex& mutex_tracks)
{
	bool endFlag = true;
	while (endFlag)
	{
		milliseconds thisTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		mutex_frames.lock();
		mutex_tracks.lock();
		for (auto frameIt = framesFlow.begin(); frameIt != framesFlow.end();)
		{
			milliseconds frameTime = frameIt->first;
			if ((thisTime.count() - frameTime.count()) > timeRange)
			{
				frameIt = framesFlow.erase(frameIt);
				for (auto trackIt = allTracks.begin(); trackIt != allTracks.end(); trackIt++)
				{
					auto mapIt = trackIt->find(frameTime);
					if (mapIt != trackIt->end())
					{
						mapIt = trackIt->erase(mapIt);
					}
				}
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

	bool endFlag = true;
	while (endFlag)
	{
		Mat frame, mask, fgimg;
		currentTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		capture >> frame;
		backgroundSubtractor(frame, mask, -1);
		mask.copyTo(savemask);
		Frame frametoMap(frame, savemask);

		mutex_frames.lock();
		mutex_tracks.lock();
		for (auto frameIt = framesFlow.begin(); frameIt != framesFlow.end();)
		{
			milliseconds frameTime = frameIt->first;
			if ((currentTime.count() - frameTime.count()) > timeRange)
			{
				frameIt = framesFlow.erase(frameIt);
				for (auto trackIt = allTracks.begin(); trackIt != allTracks.end(); trackIt++)
				{
					auto mapIt = trackIt->find(frameTime);
					if (mapIt != trackIt->end())
					{
						mapIt = trackIt->erase(mapIt);
					}
				}
			}
			else frameIt++;
		}
		framesFlow.emplace(currentTime, frametoMap);
		mutex_frames.unlock();
		mutex_tracks.unlock();

		fgimg = Scalar::all(0);
		frame.copyTo(fgimg, mask);
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
				for (auto contIt = conts.begin(); contIt != conts.end(); contIt++)
				{
					map<milliseconds, vector<Point>> oneTrack;
					oneTrack.emplace(currentTime, *contIt);
					allTracks.push_back(oneTrack);
				}
			}
			else
			{
				vector<uchar> status;
				vector<float> err;
				multimap<int, int> pointsNum;
				vector<Point2f> pointsPrev, pointsNow;
				for (auto allTrackIt = allTracks.begin(); allTrackIt != allTracks.end(); allTrackIt++)
				{
					long trackNumber = allTrackIt - allTracks.begin();
					if (allTrackIt->size() > 0)
					{
						vector<Point2f> tmpVec = getFeaturePoints(allTrackIt->rbegin()->second);
						for (auto i = pointsPrev.size(); i < pointsPrev.size() + tmpVec.size(); i++)
						{
							pointsNum.emplace(trackNumber, i);
						}
						pointsPrev.insert(pointsPrev.end(), tmpVec.begin(), tmpVec.end());
					}
				}
//                cout << "pointsPrev = " << pointsPrev.size() << endl;
			    calcOpticalFlowPyrLK(prevGray, gray, pointsPrev, pointsNow, status, err, winSize, 3, termcrit, 0, 0.001);
				for (auto allTrackIt = allTracks.begin(); allTrackIt != allTracks.end(); allTrackIt++)
				{
					long trackNumber = allTrackIt - allTracks.begin();
					if (allTrackIt->size() > 0)
					{
						auto pointsNumIt = pointsNum.equal_range(trackNumber);
						vector<Point2f> tmpVecPoints;
						for (auto it = pointsNumIt.first; it != pointsNumIt.second; it++)
						{
							tmpVecPoints.push_back(pointsNow[it->second]);
							/*
							if (status[it->second] != 0)
							{
							tmpVecPoints.push_back(pointsNow[it->second]);
							}
							*/
						}
						Rect tmpRect = boundingRect(tmpVecPoints);
						for (auto contIt = conts.begin(); contIt != conts.end();)
						{
							Rect rect = boundingRect(*contIt);
							if ((tmpRect&rect).width > 0)
							{
								allTrackIt->emplace(currentTime, *contIt);
								contIt = conts.erase(contIt);
								break;
							}
							else contIt++;
						}
					}
				}
				for (auto contIt = conts.begin(); contIt != conts.end(); contIt++)
				{
					map<milliseconds, vector<Point>> oneTrack;
					oneTrack.emplace(currentTime, *contIt);
					allTracks.push_back(oneTrack);
				}
			}
			mutex_tracks.unlock();
		}
		int delay = 1;
		waitKey(delay);
		milliseconds endtime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		cout << "cycle time - " << (endtime - currentTime).count() << endl;
		swap(prevGray, gray);
		lastTime = currentTime;
	}
}


void Capture::display(map<milliseconds, Frame>& framesFlow, mutex& mutex_frames, vector<map<milliseconds, vector<Point>>>& allTracks, mutex& mutex_tracks)
{
	bool endFlag = true;
	while (endFlag)
	{
		mutex_frames.lock();
		if (framesFlow.size() == 0)
		{
			mutex_frames.unlock();
			continue;
		}
		auto frameIt = framesFlow.rbegin();
		milliseconds time = frameIt->first;
		Mat outFrame;
		outFrame = frameIt->second.getImg();
		mutex_tracks.lock();
		for (auto trackIt = allTracks.begin(); trackIt != allTracks.end(); trackIt++)
		{
			if (trackIt->size() > 1)
			{
				auto mapIt = trackIt->find(time);
				if (mapIt==trackIt->end()) continue;
				long number = trackIt - allTracks.begin();
				Rect r = boundingRect(mapIt->second);
				rectangle(outFrame, r, Scalar(255, 0, 0), 1, 8, 0);
				stringstream ss;
				ss << number;
				string stringNumber = ss.str();
				putText(outFrame, stringNumber, Point(r.x + 5, r.y + 5),
					FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar::all(255), 1, 8);
			}
		}
        mutex_frames.unlock();
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
	putText(img, ctime(&timefordisp), Point(20, img.rows - 40), FONT_HERSHEY_COMPLEX, 1, Scalar::all(255), 1, 8);
}