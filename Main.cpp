#include "Capture.h"
#include "Detect.h"
#include "Frame.h"
#include "FileReader.h"
#include "FolderReader.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;

vector <Frame> frames;
mutex mutex_frames;
Detect detect;

const int cameraNumber = 0;
//const string fileName = R"(C:\Temp\cats\cats.avi)";
const string fileName = R"(C:\Temp\cats\768.avi)";



int main(int argc, char *argv[])
{
//	Capture capture(cameraNumber);
	Capture capture(fileName);
	if (!capture.isOpened())
	{
		cout << "Error: Camera #" << cameraNumber << " is not available now." << endl;
		return -1;
	}
	thread capturing(&Capture::find, &capture, ref(frames), ref(mutex_frames));
	thread cutting(&Capture::cut, &capture, ref(frames), ref(mutex_frames));
//	thread display(&Detect::display, &detect, ref(frames), ref(mutex_frames));
//	thread tracking(&Detect::detectPoints, &detect, ref(frames), ref(mutex_frames));

	if (capturing.joinable())
	{
		capturing.join();
	}

	if (cutting.joinable())
	{
		cutting.join();
	}


/*
	if (display.joinable())
	{
		display.join();
	}
*/

/*
	if (tracking.joinable())
	{
		tracking.join();
	}
*/


	return 0;
}

