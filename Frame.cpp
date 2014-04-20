#include "Frame.h"


Frame::Frame(chrono::milliseconds time, Mat mat, vector<vector<Point>> contours)
{
	timeStamp = time;
	img = mat;
	allContours = contours;
}


Frame::~Frame()
{
}



long long Frame::getTime()
{
	return timeStamp.count();
}

cv::Mat Frame::getImg()
{
	return img;
}

vector<vector<Point>> Frame::getContours()
{
	return allContours;
}
