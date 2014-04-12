#pragma once
#include <opencv2/opencv.hpp>
#include <mutex>
#include <vector>
#include <chrono>
#include "Square.h"


using namespace cv;
using namespace std;
using namespace chrono;

class Detect
{
	__int64 Detect::calcHammingDistance(__int64 x, __int64 y);

public:
	Detect();
	~Detect();
	void Detect::detect(vector<Square>&, mutex&);
};

