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

__int64 Detect::getHashValue(Mat src)
{
	__int64 hash;
	Mat img;
	if (src.channels() == 3)
		cvtColor(src, img, CV_BGR2GRAY);
	else
		img = src.clone();
	resize(img, img, Size(8, 8));
	int average = mean(img).val[0];
	Mat mask = (img >= (uchar)average);
	int i = 0;
	for (int y = 0; y < mask.rows; y++) {
		uchar* pData = mask.ptr <uchar>(y);
		for (int x = 0; x < mask.cols; x++) {
			if (pData[x]){
				hash |= 1i64 << i;
			}
			i++;
		}
	}
	return hash;
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

void Detect::detection(map <time_t, Mat>& squares, mutex& mutex_squares)
{
	const int range = 5;
	while (true)
	{
		time_t thisTime;
		time(&thisTime);
		mutex_squares.lock();
		//		cout << squares.size() << endl;
		for (map<time_t, Mat>::iterator iter = squares.begin(); iter != squares.end();)
		{
//			cout << thisTime - (*iter).first << " " << squares.size() << endl;
			if ((thisTime - (*iter).first) > range)
			{
//				cout << "erase " << (*iter).first << endl;
				iter = squares.erase(iter);
			}
			else
			{
				Mat image;
				//				cout << "display " << (*iter).first << endl;
				const __int64 percept = -3688730819679167236;
				__int64 res;
				Mat img;
				img = (*iter).second;
				__int64 hash = getHashValue(img);
				res = calcHammingDistance(percept, hash);
				if (res <= 4)
				{
					cout << "hand" << " " << res << endl;
					imshow("img", img);
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




