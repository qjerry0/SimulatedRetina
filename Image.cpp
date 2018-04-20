#ifndef _Image_h
#define _Image_h
#define WIDTH 128
#define HEIGHT 128

#include <vector>
#include "Image.h"
#include "Color.h"
#include <cmath>
#endif


//includes lots of code from stackoverlow post on image IO:

using namespace std;
typedef unsigned char unchar;
std::vector<std::vector<Color*>> rgbArray;

//Constructor

Image::Image(const char* fileName) {

	imageData = new unchar*[HEIGHT]; // create new array size: height of image.

	for (int i = 0; i < HEIGHT; i++) {

		imageData[i] = new unchar[WIDTH]; //create matrix.
	}

	//image I/O
	pInFile = new ifstream;
	pInFile->open(fileName, ios::in | ios::binary); // open fileName and read as binary.
	pInFile->seekg(0, ios::beg); //pos filter at beginning of image file.
	pInFile->read(reinterpret_cast<char*>(imageHeaderData), 1078); //read bmp header data into array.

	for (int i = 0; i<HEIGHT; i++) {
		pInFile->read(reinterpret_cast<char*>(imageData[i]), WIDTH);//read row into each array entry.
		//TODO: create RGB array here
	}

	pInFile->close(); //close stream.

}

Image::~Image() {

	delete pInFile;
	delete pOutFile;

	for (int i = 0; i<HEIGHT; i++) {
		delete[] imageData[i];
	}

	delete[] imageData;
}

std::vector<std::vector<Color*>> toRGB() {
	return rgbArray;
}

void Image::write(const char* fileName) {

	pOutFile = new ofstream;
	pOutFile->open(fileName, ios::out | ios::trunc | ios::binary);
	pOutFile->write(reinterpret_cast<char*>(imageHeaderData), 1078); //write header data onto output

	pOutFile->close(); //close stream
}


