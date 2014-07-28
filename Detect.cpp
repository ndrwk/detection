#include "Detect.h"
#include <mutex>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching/stitcher.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <thread>

using namespace std;
using namespace cv;

Detect::Detect()
{
}


Detect::~Detect()
{
}



void Detect::detectOnContours(vector<Frame>& frames, mutex& mutex_frames)
{
	vector<Frame> copy_frames;
	while (true)
	{

/*
		mutex_frames.lock();
		copy_frames = frames;
		mutex_frames.unlock();
		for (vector<Frame>::iterator iter = copy_frames.begin(); iter != copy_frames.end(); iter++)
		{
			Frame frame = *iter;
			vector<vector<Point>> contours;
			contours = frame.getContours();

			for (vector<vector<Point>>::iterator iterC = contours.begin(); iterC != contours.end(); iterC++)
			{
				vector<Point> cont = *iterC;
				int counter = 0;
				for (vector<Frame>::iterator iterI = copy_frames.begin(); iterI != copy_frames.end(); iterI++)
				{
					vector<vector<Point>> contsI = (*iterI).getContours();
					for (vector<vector<Point>>::iterator iterJ = contsI.begin(); iterJ != contsI.end(); iterJ++)
					{
						double match = matchShapes(cont, *iterJ, CV_CONTOURS_MATCH_I1, 0);
						if ((match < 0.2)&((iter-copy_frames.begin())==(iterI-copy_frames.begin()))) counter++;
					}
				}
//				cout << counter << endl;
				if (counter>1)
				{
					Mat pic = frame.getFgimg();
					Rect rect = boundingRect(cont);
					rectangle(pic, rect, Scalar(0, 255, 0), 3, 8, 0);
					display(pic);
				}
			}
		}
*/
		waitKey(20);
	}
}

void Detect::detectOnRects(vector<Frame>& frames, mutex& mutex_frames)
{
	vector<Frame> copy_frames;
	while (true)
	{

		mutex_frames.lock();
		copy_frames = frames;
		mutex_frames.unlock();
		for (vector<Frame>::iterator iter = copy_frames.begin(); iter != copy_frames.end(); iter++)
		{
			vector<Rect> rects = (*iter).getRects();
			Mat pic = (*iter).getImg();
			cvtColor(pic, pic, CV_BGR2GRAY);
			for (vector<Rect>::iterator iterC = rects.begin(); iterC != rects.end(); iterC++)
			{
				Rect bondRect = *iterC;
				Mat boundedPic = pic(bondRect);
				Mat edges;
//				threshold(boundedPic, boundedPic, 25, 255, CV_THRESH_BINARY);
				Canny(boundedPic, edges, 5, 200, 3);
				int counter = 0;
//				edges.copyTo(boundedPic,pic);
/*
				for (vector<Frame>::iterator iterI = copy_frames.begin(); iterI != copy_frames.end(); iterI++)
				{
					vector<vector<Point>> contsI = (*iterI).getContours();
					for (vector<vector<Point>>::iterator iterJ = contsI.begin(); iterJ != contsI.end(); iterJ++)
					{
						double match = matchShapes(cont, *iterJ, CV_CONTOURS_MATCH_I1, 0);
						if ((match < 0.2)&((iter - copy_frames.begin()) == (iterI - copy_frames.begin()))) counter++;
					}
				}
				//				cout << counter << endl;
				if (counter>1)
				{
					Mat pic = frame.getFgimg();
					Rect rect = boundingRect(cont);
					rectangle(pic, rect, Scalar(0, 255, 0), 3, 8, 0);
					display(pic);
				}
*/
			}
			imshow("123", pic);

		}
		waitKey(20);
	}
}

void Detect::display(Mat pic)
{
	imshow("pic", pic);
}


