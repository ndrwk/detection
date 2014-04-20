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



void Detect::detect(vector<Frame>& frames, mutex& mutex_frames)
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
		for (vector<Frame>::iterator iter0 = frames.begin(); iter0 != frames.end(); iter0++)
		{
			Frame frame0 = *iter0;
			vector<vector<Point>> contours0 = frame0.getContours();
			for (vector<vector<Point>>::iterator contIter0 = contours0.begin(); contIter0 != contours0.end(); contIter0++)
			{
				vector<Point> cont0 = *contIter0;
				for (vector<Frame>::iterator iter = frames.begin(); iter != frames.end(); iter++)
				{
					Frame frame = *iter;
					vector<vector<Point>> contours = frame.getContours();
					for (vector<vector<Point>>::iterator contIter = contours.begin(); contIter != contours.end(); contIter++)
					{
						vector<Point> cont = *contIter;
						double match = matchShapes(cont0, cont, CV_CONTOURS_MATCH_I1, 0);
						cout << match << endl;
					}
				}
			}
		}

		
		mutex_frames.unlock();
		//		this_thread::sleep_for(chrono::milliseconds(20));
		waitKey(20);
	}
}

void Detect::training(vector<Frame>& squares, mutex& mutex_squares)
{
				/*
				string filename = "c:\\temp\\opencv\\";
				char hashtxt[256] = "";
				__int64 hash = square.getHash();
				_i64toa(hash, hashtxt, 10);
				filename = filename + hashtxt + ".png";
				//			cout << filename << endl;
				try
				{
					imwrite(filename, square.getImg());
				}
				catch (runtime_error& ex)
				{
					fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
					break;
				}
				*/
		
		mutex_squares.unlock();
		waitKey(20);
	
}




