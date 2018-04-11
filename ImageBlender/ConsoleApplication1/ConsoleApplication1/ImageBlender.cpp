// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "SFML\Graphics.hpp"

//i is row position, j is column position

sf::Color getClosestNeighborColor(int i, int j, sf::Image image) {
	int radius = 0;
	int colorsFound = 0;
	sf::Color oc = sf::Color::Black;

	while (true) {
		for (int k = i - radius; k <= i + radius; k++) {
			for (int m = j - radius; m <= j + radius; m++) {
				if ((m - j)*(m - j) + (k - i)*(k - i) < radius*radius && (m >= 0 && m < image.getSize().x && k >= 0 && k < image.getSize().y)) {
					if (image.getPixel(m, k).a != 0) {
						sf::Color newColor = image.getPixel(m, k);
						oc = sf::Color((oc.r*colorsFound + newColor.r) / (colorsFound + 1), (oc.g*colorsFound + newColor.g) / (colorsFound + 1), (oc.b*colorsFound + newColor.b) / (colorsFound + 1));
						colorsFound++;
					}
				}
			}
		}
		if (colorsFound > 0)
			return oc;
		else {
			radius++;
			if (radius * radius > image.getSize().x * image.getSize().x + image.getSize().y * image.getSize().y)
				return sf::Color::Black;
		}
	}
}

int main()
{
	while (true) {
		std::cout << "Enter Image Name: \n";
		std::string fileName;
		std::getline(std::cin, fileName);

		sf::Image inputImage;
		bool imageLoaded = inputImage.loadFromFile("Input/" + fileName);

		if (!imageLoaded) {
			std::cout << "Load Failed!\n";
			return 0;
		}

		for (int i = 0; i < inputImage.getSize().y; i++) {
			for (int j = 0; j < inputImage.getSize().x; j++) {
				sf::Color pixColor = inputImage.getPixel(j, i);
				if (pixColor.a == 0) {
					inputImage.setPixel(j, i, getClosestNeighborColor(i, j, inputImage));
				}

				if ((i*inputImage.getSize().x + j) * 100 / (inputImage.getSize().x*inputImage.getSize().y) > (i*inputImage.getSize().x + j) * 100 / (inputImage.getSize().x*inputImage.getSize().y)) {
					std::cout << (i*inputImage.getSize().x + j) * 100 / (inputImage.getSize().x*inputImage.getSize().y) << "% complete.";
				}
			}
		}

		inputImage.saveToFile("Output/" + fileName);

		std::cout << "Enter another image? (1: yes, 0: no)\n";
		int result;
		std::cin >> result;
		std::cin.ignore(100, '\n');
		if (result == 0)
			return 0;
	}
}

