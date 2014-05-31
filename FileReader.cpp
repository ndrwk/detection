#include "FileReader.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
using namespace cv;


FileReader::FileReader(string filename)
{
	name = filename;
	string delimiter = " ";
	ifstream file(filename);
	string textLine;
	getline(file, textLine);
	int pos = 0;
	string token;
	while ((pos = textLine.find(delimiter)) != string::npos)
	{
		token = textLine.substr(0, pos);
		int tmpInt;
		istringstream istr(token);
		istr >> tmpInt;
		coordinates.push_back(tmpInt);
		textLine.erase(0, pos + delimiter.length());
	}
	file.close();
}

FileReader::~FileReader()
{
}

vector<int> FileReader::getCoordinates()
{
	return coordinates;
}

void FileReader::showPic(Mat pic)
{
	if (coordinates.size() == 19)
	{
		LeftEye = Point(coordinates[1], coordinates[2]);
		RightEye = Point(coordinates[3], coordinates[4]);
		Mouth = Point(coordinates[5], coordinates[6]);
		LeftEar1 = Point(coordinates[7], coordinates[8]);
		LeftEar2 = Point(coordinates[9], coordinates[10]);
		LeftEar3 = Point(coordinates[11], coordinates[12]);
		RightEar1 = Point(coordinates[13], coordinates[14]);
		RightEar2 = Point(coordinates[15], coordinates[16]);
		RightEar3 = Point(coordinates[17], coordinates[18]);
	}
	circle(pic, LeftEye, 10, Scalar(255,0,0), 3);
	circle(pic, RightEye, 10, Scalar(255, 0, 0), 3);
	circle(pic, Mouth, 10, Scalar(255, 0, 0), 3);
	circle(pic, LeftEar1, 10, Scalar(255, 0, 0), 3);
	circle(pic, LeftEar2, 10, Scalar(255, 0, 0), 3);
	circle(pic, LeftEar3, 10, Scalar(255, 0, 0), 3);
	circle(pic, RightEar1, 10, Scalar(255, 0, 0), 3);
	circle(pic, RightEar2, 10, Scalar(255, 0, 0), 3);
	circle(pic, RightEar3, 10, Scalar(255, 0, 0), 3);
	putText(pic, name, Point(20, pic.rows - 40), FONT_HERSHEY_COMPLEX, 0.5, Scalar::all(255), 1, 8);
	imshow("pic", pic);
	waitKey(300);
}
