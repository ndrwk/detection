#pragma once
#include <opencv2/opencv.hpp>
#include <string>
using namespace std;
using namespace cv;

class PicLoader
{
	Mat pic;
public:
	PicLoader(string);
	~PicLoader();
	Mat getMat();
};

