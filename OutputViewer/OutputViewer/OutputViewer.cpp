// OutputViewer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "SFML\Graphics.hpp"
#include "Quadtree.h"

int main()
{
	Quadtree<float> floatTree;
	sf::RenderWindow programWindow(sf::VideoMode(400,400), "Retina Viewer");

	std::string line;
	std::ifstream myfile("ActivationData.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			floatTree
		}
		myfile.close();
	}

    return 0;
}

