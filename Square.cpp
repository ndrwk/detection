#include "Square.h"


Square::Square(chrono::milliseconds time, Mat mat)
{
	timeStamp = time;
	img = mat;
	hash = getHashValue(img);
}


Square::~Square()
{
}

__int64 Square::getHashValue(Mat src)
{
	__int64 hash;
	Mat img;
	if (src.channels() == 3)
		cvtColor(src, img, CV_BGR2GRAY);
	else
		img = src.clone();
	resize(img, img, Size(8, 8));
	int average = mean(img).val[0];
	Mat mask = (img >= (uchar)average);
	int i = 0;
	for (int y = 0; y < mask.rows; y++) {
		uchar* pData = mask.ptr <uchar>(y);
		for (int x = 0; x < mask.cols; x++) {
			if (pData[x]){
				hash |= 1i64 << i;
			}
			i++;
		}
	}
	return hash;
}


__int64 Square::getTime()
{
	return timeStamp.count();
}

cv::Mat Square::getImg()
{
	return img;
}

__int64 Square::getHash()
{
	return hash;
}
