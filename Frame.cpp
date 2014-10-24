#include "Frame.h"


Frame::Frame(Mat pic, Mat picMask)
{
	img = pic;
	mask = picMask;
}


Frame::~Frame()
{
}


Mat Frame::getImg()
{
	return img;
}

Mat Frame::getMask()
{
	return mask;
}