#include "Capture.h"
#include "Detect.h"
#include "Square.h"
#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include <thread>
#include <mutex>



using namespace std;
using namespace cv;


vector <Square> squares;
mutex mutex_squares;
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
	thread capturing(&Capture::getFound, &capture, ref(squares), ref(mutex_squares));
	thread detecting(&Detect::training, &detect, ref(squares), ref(mutex_squares));

	if (capturing.joinable())
	{
		capturing.join();
	}
	if (detecting.joinable())
	{
		detecting.join();
	}
	return 0;
}

