#include "Frame.h"


Frame::Frame(chrono::milliseconds time, Mat mat, vector<Rect> rects, Mat mat2)
{
	timeStamp = time;
	img = mat;
	fgimg = mat2;
	allRects = rects;
}


Frame::~Frame()
{
}



long long Frame::getTime()
{
	return timeStamp.count();
}

Mat Frame::getImg()
{
	return img;
}

Mat Frame::getFgimg()
{
	return fgimg;
}

vector<Rect> Frame::getRects()
{
	return allRects;
}

vector<Point2f> Frame::getPoints()
{
	return allPoints;
}
