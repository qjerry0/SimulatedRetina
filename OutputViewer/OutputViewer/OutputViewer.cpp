// OutputViewer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
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

Region* computeNewRenderRegion(Region rregion, float zoom, Point zoomPoint, Point windhalfsize) {
	Point newSize(rregion.halfSize.x/zoom,rregion.halfSize.y/zoom);
	double newCenterPointX = rregion.halfSize.x*(2*zoomPoint.x / windhalfsize.x - 1)*((zoom - 1.0) / zoom) + rregion.center.x;
	double newCenterPointY = rregion.halfSize.y*(2*zoomPoint.y / windhalfsize.y - 1)*((zoom - 1.0) / zoom) + rregion.center.y;
	return new Region(Point(newCenterPointX, newCenterPointY), newSize);
}

Point toPixelSpaceCoordinates(Data<MiniOpponent> dop, Region rendRegion, Point outputImageSize) {
	double minX = rendRegion.center.x - rendRegion.halfSize.x;
	double minY = rendRegion.center.y - rendRegion.halfSize.y;
	return Point(((dop.pos.x - minX) / (2 * rendRegion.halfSize.x))*outputImageSize.x, ((dop.pos.y - minY) / (2 * rendRegion.halfSize.y))*outputImageSize.y);
}

void renderOutput(Quadtree<MiniOpponent>* tree, sf::Sprite* sprite, sf::Texture* tex, OpponentType type, Region rendRegion, Point windhalfsize) {
	sf::Image outputImage;
	outputImage.create(rendRegion.halfSize.x*2, rendRegion.halfSize.y*2, sf::Color(0, 0, 0, 0));
	std::vector<Data<MiniOpponent>> opsData = tree->queryRange(rendRegion);
	Node* usedPixelsRoot = newNode(opsData[0].pos);
	
	bool first = true;
	for (Data<MiniOpponent> dop : opsData){
		if (first) {
			first = false;
			continue;
		}
		Point dopPixCoords = toPixelSpaceCoordinates(dop, rendRegion, Point(outputImage.getSize().x, outputImage.getSize().y));
		Node* pixel = search(usedPixelsRoot, dopPixCoords);
		if (pixel == NULL) {
			usedPixelsRoot = insert(usedPixelsRoot, dopPixCoords);
			pixel = search(usedPixelsRoot, dopPixCoords);
		}

		double output = dop.load->output;
		FloatColor fc = FloatColor(0,0,0);
		switch (type) {
		case OpponentType::Luminance:
			fc = fc + FloatColor(output , output, output);
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
		pixel->currentColor = pixel->currentColor * (num - 1) / num + fc/num;
		outputImage.setPixel(dopPixCoords.x, dopPixCoords.y, FloatColor::toSFMLColor(pixel->currentColor));
	}

	tex->loadFromImage(outputImage);
	sprite->setTexture(*tex);
	sprite->setScale(windhalfsize.x / outputImage.getSize().x, windhalfsize.y / outputImage.getSize().y);
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

		Region treeBoundary(Point(std::stod(substr),std::stod(substr2)), Point(std::stod(substr3), std::stod(substr4)));
		lumTree = Quadtree<MiniOpponent>(treeBoundary);
		rgTree = Quadtree<MiniOpponent>(treeBoundary);
		byTree = Quadtree<MiniOpponent>(treeBoundary);
		
		while (getline(myfile, line))
		{
			Data<MiniOpponent> datOp;
			datOp.load = new MiniOpponent(0,OpponentType::Luminance);
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

			if(datOp.load->opType == OpponentType::Luminance)
				lumTree.insert(datOp);
			else if (datOp.load->opType == OpponentType::RedGreen)
				rgTree.insert(datOp);
			else if (datOp.load->opType == OpponentType::BlueYellow)
				byTree.insert(datOp);
		}
		myfile.close();
	}

	std::cout << "Succesfully inserted and queried " << lumTree.queryRange(lumTree.getBoundary()).size() << " luminance opponents!\n";

	Quadtree<MiniOpponent>* tree_ptr = &lumTree;
	OpponentType renderType = OpponentType::Luminance;
	Region renderRegion = lumTree.getBoundary();
	float zoomLevel = 1;
	float zoomRatio;
	Point zoomPoint;

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

		if(changedRender || firstLoop)
			renderOutput(tree_ptr, &outputSprite, &outputTexture, renderType, renderRegion, windowSize);

		programWindow.clear();
		programWindow.draw(outputSprite);
		programWindow.display();
		firstLoop = false;
	}

    return 0;
}

