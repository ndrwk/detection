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



ContStruct::~ContStruct()
{
}
