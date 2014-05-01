#include "Capture.h"
#include "Detect.h"
#include "Frame.h"
#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include <thread>
#include <mutex>



using namespace std;
using namespace cv;


vector <Frame> frames;
mutex mutex_frames;
Detect detect;

const int cameraNumber = 0;


int main(int argc, char *argv[])
{
	Capture capture(cameraNumber);
	if (!capture.isOpened())
	{
		cout << "Error: Camera #" << cameraNumber << " is not available now." << endl;
		return -1;
	}
	thread capturing(&Capture::find, &capture, ref(frames), ref(mutex_frames));
	thread cutting(&Capture::cut, &capture, ref(frames), ref(mutex_frames));
	thread detecting(&Detect::detectOnRects, &detect, ref(frames), ref(mutex_frames));

	if (capturing.joinable())
	{
		capturing.join();
	}

	if (cutting.joinable())
	{
		cutting.join();
	}

	if (detecting.joinable())
	{
		detecting.join();
	}

	return 0;
}

