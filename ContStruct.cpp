#include "ContStruct.h"


ContStruct::ContStruct(int n, vector<Point> c, vector<Point2f> rp)
{
	repers = rp;
	contour = c;
	number = n;
}



int ContStruct::getNumber()
{
	return number;
}

Rect ContStruct::getRect()
{
	return boundingRect(contour);
}

vector<Point2f> ContStruct::getRepers()
{
	return repers;
}

vector<Point> ContStruct::getContiur()
{
	return contour;
}


ContStruct::~ContStruct()
{
}
