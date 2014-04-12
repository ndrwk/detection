#include "Detect.h"
#include <mutex>
#include <opencv2/opencv.hpp>
#include <thread>

using namespace std;
using namespace cv;

Detect::Detect()
{
}


Detect::~Detect()
{
}


__int64 Detect::calcHammingDistance(__int64 x, __int64 y)
{
	__int64 dist = 0, val = x ^ y;
	while (val)
	{
		++dist;
		val &= val - 1;
	}
	return dist;
}

void Detect::detect(vector<Square>& squares, mutex& mutex_squares)
{
	const int rangeInmillisec = 3000;
	while (true)
	{
		mutex_squares.lock();
		for (vector<Square>::iterator iter = squares.begin(); iter != squares.end();)
		{
			Square square = *iter;
			milliseconds timeNow = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
			if ((timeNow.count() - square.getTime()) > rangeInmillisec)
			{
//				cout << "erase " << (*iter).first << endl;
				iter = squares.erase(iter);
			}
			else
			{
				Mat image;
				//				cout << "display " << (*iter).first << endl;
				const __int64 percept = -3688730819679167236;
				__int64 hash = square.getHash();
				__int64 res = calcHammingDistance(percept, hash);
				if (res <= 4)
				{
					cout << "hand" << " " << res << endl;
					imshow("img", square.getImg());
				}
				iter++;

			}
		}
			mutex_squares.unlock();
			//		this_thread::sleep_for(chrono::milliseconds(20));
			waitKey(20);
		}
	}



	/*
	__int64 percept = -3688730819679167236;
	__int64 res;
	for (vector<Mat>::iterator iter = squares.begin(); iter != squares.end(); ++iter)
	{
	Mat img;
	img = *iter;
	__int64 hash = getHashValue(img);
	res = calcHammingDistance(percept, hash);
	if (res <= 4)
	{
	cout << "hand" << " " << res << endl;
	imshow("img", img);
	}


	/*
	string filename = "c:\\temp\\opencv\\";
	char hashtxt[256] = "";
	_i64toa(hash, hashtxt, 10);
	filename = filename + hashtxt + ".png";
	//			cout << filename << endl;
	try {
	imwrite(filename, img);
	}
	catch (runtime_error& ex) {
	fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
	return 1;
	}

	}
	*/




