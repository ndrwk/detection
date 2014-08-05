#include "Vortex.h"


Vortex::Vortex(int num, Point2f pPrev, Point2f pNow)
{
	number = num;
	p0 = pPrev;
	p1 = pNow;
	float dX = p1.x - p0.x;
	float dY = p1.y - p0.y;
	lenght = (int)sqrt(dX*dX + dY*dY);
	const float M_PI = 3.1416;
	float atn = atan(abs(dY) / abs(dX));
	float tmp;
	if (dX > 0 && dY > 0)
	{
		tmp = atn;
	}
	else if (dX < 0 && dY>0)
	{
		tmp = (M_PI - atn);
	}
	else if (dX < 0 && dY < 0)
	{
		tmp = (M_PI + atn);
	}
	else
	{
		tmp = (2 * M_PI - atn);
	}
	angle = (int) tmp * 254 / (2 * M_PI);
}


Vortex::~Vortex()
{
}


int Vortex::getAngle()
{
	return angle;
}

int Vortex::getLenght()
{
	return lenght;
}


Point2f Vortex::getP0()
{
	return p0;
}

Point2f Vortex::getP1()
{
	return p1;
}

int Vortex::getNumber()
{
	return number;
}

