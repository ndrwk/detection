#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Vortex
{
	Point2f p0, p1;
	int lenght;
	int angle;
	int number;
public:
	Vortex(int, Point2f, Point2f);
	Point2f getP0();
	Point2f getP1();
	int getNumber();
	int getLenght();
	int getAngle();

	static bool Vortex::cmpByLengh(Vortex a, Vortex b)
	{
		return a.getLenght() < b.getLenght();
	}

	static bool cmpByAngle(Vortex a , Vortex b)
	{
		return a.getAngle() < b.getAngle();
	}

	~Vortex();
};

