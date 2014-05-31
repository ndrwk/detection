#include "PicLoader.h"
#include <opencv2/opencv.hpp>
#include <string>
using namespace std;
using namespace cv;


PicLoader::PicLoader(string filename)
{
	pic = imread(filename);
}


PicLoader::~PicLoader()
{
}

Mat PicLoader::getMat()
{
	return pic;
}