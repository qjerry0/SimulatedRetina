// OutputViewer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include "SFML\Graphics.hpp"
#include "Quadtree.h"
#include "FloatColor.h"
#include "ppl.h"
#include <sstream>
#include <array>
#include <set>
#include "KDTree.h"
#include <chrono>

enum class OpponentType { Luminance, RedGreen, BlueYellow };
struct MiniOpponent {
	double output;
	OpponentType opType;

	MiniOpponent(double out, OpponentType oc) : output(out), opType(oc) {}
};

struct PointInt {
	int x;
	int y;

	PointInt(int x, int y) : x(x), y(y) {}
};

class Timer
{
public:
	Timer() : beg_(clock_::now()) {}
	void reset() { beg_ = clock_::now(); }
	double elapsed() const {
		return std::chrono::duration_cast<second_>
			(clock_::now() - beg_).count();
	}

private:
	typedef std::chrono::high_resolution_clock clock_;
	typedef std::chrono::duration<double, std::ratio<1> > second_;
	std::chrono::time_point<clock_> beg_;
};

/*Calculates the new render region using the old region, the zoom location, the window half size, and a zoom amount
  Zooming is done such that the zoom location (zoomPoint) is kept at the same location on the screen as it was before the zoom*/

Region* computeNewRenderRegion(Region rregion, float zoom, Point zoomPoint, Point windhalfsize) {
	Point newSize(rregion.halfSize.x / zoom, rregion.halfSize.y / zoom);
	double newCenterPointX = rregion.halfSize.x*(2 * zoomPoint.x / windhalfsize.x - 1)*((zoom - 1.0) / zoom) + rregion.center.x;
	double newCenterPointY = rregion.halfSize.y*(2 * zoomPoint.y / windhalfsize.y - 1)*((zoom - 1.0) / zoom) + rregion.center.y;
	return new Region(Point(newCenterPointX, newCenterPointY), newSize);
}

/*Converts coordinates in the retina space to coordinates in the pixel space of the current render region (if the render region is the
 top left fourth of the retina, the pixel space coordinates of the center of the retina will be those of the bottom right corner of the window)*/

Point toPixelSpaceCoordinates(Data<MiniOpponent> dop, Region rendRegion, Point outputImageSize) {
	double minX = rendRegion.center.x - rendRegion.halfSize.x;
	double minY = rendRegion.center.y - rendRegion.halfSize.y;
	return Point(((dop.pos.x - minX) / (2 * rendRegion.halfSize.x))*outputImageSize.x, ((dop.pos.y - minY) / (2 * rendRegion.halfSize.y))*outputImageSize.y);
}

/*Generates an image based on the render region, current quadtree of opponents, and window size, and then appropriately updates the dereferenced arguments
"sprite" and "tex" so that the image can be displayed by the caller*/

void renderOutput(Quadtree<MiniOpponent>* tree, sf::Sprite* sprite, sf::Texture* tex, OpponentType type, Region rendRegion, Point windsize) {
	sf::Image outputImage;
	outputImage.create(windsize.x / 2, windsize.y / 2, sf::Color(0, 0, 0, 0));
	std::vector<Data<MiniOpponent>> opsData = tree->queryRange(rendRegion);
	Point dopPixCoords = toPixelSpaceCoordinates(opsData[0], rendRegion, Point(outputImage.getSize().x, outputImage.getSize().y));
	Node* usedPixelsRoot = newNode(dopPixCoords);

	bool first = true;
	for (Data<MiniOpponent> dop : opsData) {
		if (first) {
			first = false;
			continue;
		}
		dopPixCoords = toPixelSpaceCoordinates(dop, rendRegion, Point(outputImage.getSize().x, outputImage.getSize().y));
		Node* pixel = search(usedPixelsRoot, dopPixCoords);
		if (pixel == NULL) {
			usedPixelsRoot = insert(usedPixelsRoot, dopPixCoords);
			pixel = search(usedPixelsRoot, dopPixCoords);
		}

		double output = dop.load->output;
		FloatColor fc = FloatColor(0, 0, 0);
		switch (type) {
		case OpponentType::Luminance:
			fc = fc + FloatColor(output, output, output);
			break;
		case OpponentType::RedGreen:
			fc = fc + FloatColor(output > 0 ? output : 0, output < 0 ? -output : 0, 0);
			break;
		case OpponentType::BlueYellow:
			fc = fc + FloatColor(output > 0 ? output : 0, output > 0 ? output : 0, output < 0 ? -output : 0);
			break;
		}

		pixel->numThingsWithin++;
		int num = pixel->numThingsWithin;
		pixel->currentColor = pixel->currentColor * (num - 1) / num + fc / num;
		outputImage.setPixel(dopPixCoords.x, dopPixCoords.y, FloatColor::toSFMLColor(pixel->currentColor));
	}

	tex->loadFromImage(outputImage);
	sprite->setTexture(*tex);
	sprite->setScale(2, 2);
}

int main()
{
	Quadtree<MiniOpponent> lumTree;
	Quadtree<MiniOpponent> rgTree;
	Quadtree<MiniOpponent> byTree;
	std::string line;
	std::ifstream myfile("OpponentActivationData.txt");
	if (myfile.is_open())
	{
		getline(myfile, line);
		std::stringstream sstream(line);
		std::string substr;
		std::string substr2;
		std::string substr3;
		std::string substr4;

		getline(sstream, substr, ',');
		getline(sstream, substr2, ',');
		getline(sstream, substr3, ',');
		getline(sstream, substr4, ',');

		Region treeBoundary(Point(std::stod(substr), std::stod(substr2)), Point(std::stod(substr3), std::stod(substr4)));
		lumTree = Quadtree<MiniOpponent>(treeBoundary);
		rgTree = Quadtree<MiniOpponent>(treeBoundary);
		byTree = Quadtree<MiniOpponent>(treeBoundary);

		for (int i = 0; getline(myfile, line); i++)
		{
			Data<MiniOpponent> datOp;
			datOp.load = new MiniOpponent(0, OpponentType::Luminance);
			sstream = std::stringstream(line);

			for (int i = 0; sstream.good(); i++) {
				getline(sstream, substr, ',');
				switch (i) {
				case 0:
					datOp.pos.x = std::stod(substr.substr(1));
					break;
				case 1:
					datOp.pos.y = std::stod(substr);
					break;
				case 2:
					switch (substr[0]) {
					case 'L':
						datOp.load->opType = OpponentType::Luminance;
						break;
					case 'R':
						datOp.load->opType = OpponentType::RedGreen;
						break;
					case 'B':
						datOp.load->opType = OpponentType::BlueYellow;
						break;
					}
					break;
				case 3:
					datOp.load->output = std::stod(substr.substr(0, substr.length() - 2));
					break;
				default:
					break;
				}
			}

			if (datOp.load->opType == OpponentType::Luminance)
				lumTree.insert(datOp);
			else if (datOp.load->opType == OpponentType::RedGreen)
				rgTree.insert(datOp);
			else if (datOp.load->opType == OpponentType::BlueYellow)
				byTree.insert(datOp);

			if (i % 10000 == 9999) {
				std::cout << "\r" << i + 1 << " Opponents Inserted.";
			}
		}
		std::cout << "\n";
		myfile.close();
	}

	std::cout << "Succesfully inserted and queried " << lumTree.queryRange(lumTree.getBoundary()).size() << " luminance opponents!\n";

	Quadtree<MiniOpponent>* tree_ptr = &lumTree;
	OpponentType renderType = OpponentType::Luminance;
	Region renderRegion = lumTree.getBoundary();
	float zoomLevel = 1;
	float zoomRatio;
	Point zoomPoint;
	std::string fileName;

	sf::Color windowColor = sf::Color::Black;
	sf::Texture outputTexture;
	sf::Sprite outputSprite;

	sf::RenderWindow programWindow(sf::VideoMode(960, 960), "Retina Viewer");
	Point windowSize(programWindow.getSize().x, programWindow.getSize().y);

	bool firstLoop = true;
	while (programWindow.isOpen()) {
		sf::Event windowevent;
		bool changedRender = false;
		while (programWindow.pollEvent(windowevent))
		{
			if (windowevent.type == sf::Event::Closed)
				programWindow.close();

			if (windowevent.type == sf::Event::EventType::KeyPressed) {
				switch (windowevent.key.code) {
				case sf::Keyboard::Num1:
					renderType = OpponentType::Luminance;
					if (tree_ptr != &lumTree)
						changedRender = true;
					tree_ptr = &lumTree;
					break;
				case sf::Keyboard::Num2:
					renderType = OpponentType::RedGreen;
					if (tree_ptr != &rgTree)
						changedRender = true;
					tree_ptr = &rgTree;
					break;
				case sf::Keyboard::Num3:
					renderType = OpponentType::BlueYellow;
					if (tree_ptr != &byTree)
						changedRender = true;
					tree_ptr = &byTree;
					break;
				case sf::Keyboard::C:
					std::cout << "Image File Name: ";
					getline(std::cin, fileName);
					outputTexture.copyToImage().saveToFile("Output/" + fileName + ".png");
					break;
				case sf::Keyboard::W:
					windowColor = sf::Color::White;
					break;
				case sf::Keyboard::B:
					windowColor = sf::Color::Black;
					break;
				case sf::Keyboard::G:
					windowColor = sf::Color(128,128,128);
					break;
				case sf::Keyboard::Z:
					std::cout << "Enter Zoom Amount: ";
					std::cin >> zoomRatio;
					std::cin.ignore(100, '\n');
					zoomLevel = zoomLevel * zoomRatio;
					std::cout << "Enter Zoom Point X (fraction of screen width): ";
					float zoomX, zoomY;
					std::cin >> zoomX;
					std::cin.ignore(100, '\n');
					std::cout << "Enter Zoom Point Y (fraction of screen width): ";
					std::cin >> zoomY;
					std::cin.ignore(100, '\n');
					zoomPoint = Point(windowSize.x * zoomX, windowSize.y*zoomY);
					renderRegion = *computeNewRenderRegion(renderRegion, zoomRatio, zoomPoint, windowSize);
					changedRender = true;
					break;
				}
			}

			if (windowevent.type == sf::Event::EventType::MouseWheelMoved) {
				if (windowevent.mouseWheel.delta > 0) {
					for (int i = windowevent.mouseWheel.delta; i > 0; i--) {
						zoomRatio = zoomLevel;
						zoomLevel += (((float)((int)zoomLevel)) / 10);
						zoomRatio = zoomLevel / zoomRatio;
						zoomPoint = Point(windowevent.mouseWheel.x, windowevent.mouseWheel.y);
						renderRegion = *computeNewRenderRegion(renderRegion, zoomRatio, zoomPoint, windowSize);
						changedRender = true;
					}
				}
				else {
					for (int i = -windowevent.mouseWheel.delta; i > 0; i--) {
						zoomRatio = zoomLevel;
						zoomLevel -= (((float)((int)zoomLevel)) / 10);
						if (zoomLevel < 1)
							zoomLevel = 1;
						zoomRatio = zoomLevel / zoomRatio;
						zoomPoint = Point(windowevent.mouseWheel.x, windowevent.mouseWheel.y);
						renderRegion = *computeNewRenderRegion(renderRegion, zoomRatio, zoomPoint, windowSize);
						changedRender = true;
					}
				}
			}
		}

		if (changedRender || firstLoop)
			renderOutput(tree_ptr, &outputSprite, &outputTexture, renderType, renderRegion, windowSize);

		programWindow.clear(windowColor);
		programWindow.draw(outputSprite);
		programWindow.display();
		firstLoop = false;
	}

	return 0;
}