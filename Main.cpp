#include "Capture.h"
#include <thread>
#include <fstream>

using namespace std;
using namespace cv;

map<milliseconds, Frame> frames;
vector<map<milliseconds, vector<Point>>> allTracks;
mutex mutex_frames, mutex_tracks;
//Detect detect;

const int cameraNumber = 0;
//const string fileName = R"(C:\Temp\cats\cats.avi)";
const string fileName = R"(/home/drew/ClionProjects/detection/123.avi)";
const string picname = R"(/home/drew/ClionProjects/hello/img.jpg)";



int main(int argc, char *argv[])
{
	Capture capture(cameraNumber);

    FILE* f = fopen("/home/drew/ClionProjects/detection/123.avi", "r");
    if (f == NULL)
        std::cout << "Error ! File does not exist!" << std::endl;

//	Mat img;
//	img = imread(picname, 1);
//	if (img.empty()) cout << "Пусто " << picname << endl;
//
//	imshow("image", img);
//	waitKey(0);


//	Capture capture(fileName);
//	if (!capture.isOpened())
//	{
//		cout << "Error: Camera #" << cameraNumber << " is not available now." << endl;
//		return -1;
//	}
	thread capturing(&Capture::find, &capture, ref(frames), ref(mutex_frames), ref(allTracks), ref(mutex_tracks));
//	thread cutting(&Capture::cut, &capture, ref(frames), ref(mutex_frames), ref(allTracks), ref(mutex_tracks));
	thread display(&Capture::display, &capture, ref(frames), ref(mutex_frames), ref(allTracks), ref(mutex_tracks));

	if (capturing.joinable())
	{
		capturing.join();
	}

//	if (cutting.joinable())
//	{
//		cutting.join();
//	}

	if (display.joinable())
	{
		display.join();
	}


/*
	if (tracking.joinable())
	{
		tracking.join();
	}
*/


	return 0;
}

