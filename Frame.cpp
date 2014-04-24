#include "Frame.h"


Frame::Frame(chrono::milliseconds time, Mat mat, vector<Rect> rects)
{
	timeStamp = time;
	img = mat;
	allRects = rects;
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

vector<Rect> Frame::getRects()
{
	return allRects;
}
