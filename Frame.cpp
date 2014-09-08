#include "Frame.h"


Frame::Frame(chrono::milliseconds time, Mat pic, Mat picMask)
{
	timeStamp = time;
	img = pic;
	mask = picMask;
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

Mat Frame::getMask()
{
	return mask;
}
