#include "Capture.h"
#include <thread>

using namespace std;
using namespace cv;

map<milliseconds, Frame> frames;
vector<map<milliseconds, vector<Point>>> allTracks;
mutex mutex_frames, mutex_tracks;

const int cameraNumber = 0;
const string fileName = R"(/home/drew/ClionProjects/detection/123.avi)";
//const string fileName = R"(/home/drew/ClionProjects/detection/TB.mp4)";


int main()
{
//	Capture capture(cameraNumber);
//	if (!capture.isOpened())
//	{
//		cout << "Error: Camera #" << cameraNumber << " is not available now." << endl;
//		return -1;
//	}

	const char* file = fileName.c_str();
	FILE* f = fopen(file, "r");
	if (f == NULL) {
		cout << "Error: File "<< file << " is not found." << endl;
		return -1;
	}
	fclose(f);
	Capture capture(fileName);


	thread capturing(&Capture::find, &capture, ref(frames), ref(mutex_frames), ref(allTracks), ref(mutex_tracks));
	thread display(&Capture::display, &capture, ref(frames), ref(mutex_frames), ref(allTracks), ref(mutex_tracks));

	if (capturing.joinable())
	{
		capturing.join();
	}


	if (display.joinable())
	{
		display.join();
	}


	return 0;
}

