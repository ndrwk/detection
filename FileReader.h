#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
using namespace std;
using namespace cv;

class FileReader
{
	vector <int> coordinates;
	int points;
	Point LeftEye, RightEye, Mouth, LeftEar1, LeftEar2, LeftEar3, RightEar1, RightEar2, RightEar3;
	string name;

public:
	FileReader(string filename);
	~FileReader();
	vector<int> getCoordinates();
	void showPic(Mat);
};

