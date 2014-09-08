#include "TrackPoint.h"


TrackPoint::TrackPoint(milliseconds time,  Rect r)
{
	timeStamp = time;
	rect = r;
}


TrackPoint::~TrackPoint()
{
}

Point2f TrackPoint::getCenter()
{
	return Point2f(rect.x + rect.width / 2, rect.y + rect.height / 2);
}