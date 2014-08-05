#include "RectStruct.h"


RectStruct::RectStruct(int n, Rect rn, set<int> p)
{
	rectNow = rn;
	number = n;
	points = p;
}

set<int> RectStruct::getSetOfPoints()
{
	return points;
}

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
