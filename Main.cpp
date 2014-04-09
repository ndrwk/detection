#include "Capture.h"
#include "Detect.h"
#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include <thread>
#include <mutex>
#include <map>
#include <time.h>



using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
//	vector <Mat> squares;
	map<time_t, Mat> squares;
	mutex mutex_squares;
	Capture capture(0);
	Detect detect;
	if (!capture.isOpened())
	{
		return -1;
	}
	thread thread_found(&Capture::getFound, &capture, ref(squares), ref(mutex_squares));
	thread thread_detect(&Detect::detection, &detect, ref(squares), ref(mutex_squares));

	if (thread_found.joinable())
	{
		thread_found.join();
	}
	if (thread_detect.joinable())
	{
		thread_detect.join();
	}
	return 0;
}

