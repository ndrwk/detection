#include "RectStruct.h"


RectStruct::RectStruct(int n, Rect rn)
{
	rectNow = rn;
	number = n;
}




/*
RectStruct::RectStruct(int n, Rect rn, vector<Point2f> p)
{
	rectNow = rn;
	number = n;
	points = p;
}

vector<Point2f> RectStruct::getSetOfPoints()
{
	return points;
}
*/

int RectStruct::getNumber()
{
	return number;
}

Rect RectStruct::getRect()
{
	return rectNow;
}

/*
RectStruct::RectStruct()
{
}
*/


RectStruct::~RectStruct()
{
}
