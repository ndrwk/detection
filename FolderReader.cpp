#include "FolderReader.h"
#include "FileReader.h"
#include "PicLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;


FolderReader::FolderReader(string path, string filemame)
{
	ifstream file(path + filemame);
	string textLine;
	while (!file.eof())
	{
		getline(file, textLine);
		if (textLine.length() == 0) break;
		PicLoader p_loader(path + textLine);
		getline(file, textLine);
		FileReader reader(path + textLine);
		reader.showPic(p_loader.getMat());
	}
	file.close();


}


FolderReader::~FolderReader()
{
}
