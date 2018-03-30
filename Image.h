#pragma once
#include <fstream>

using namespace std;
typedef unsigned char unchar;

// type sig taken in part from post on stackoverflow

class Image {

public:
	Image(const char* fileName); 
	~Image();
	void write(const char* fileName);
private:
	ifstream* pInFile;
	ofstream* pOutFile;
	unchar imageHeaderData[1078];
	unchar** imageData;
};