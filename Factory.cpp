#include "stdafx.h"
#include <random>
#include <ctime>
#include <iostream>
#include "Factory.h"

const float Factory::size = 100.0f;

//Makes a size x size grid of Cones with types randomly chosen with the following probabilities
//Red: 60%
//Green: 30%
//Blue: 10%

Quadtree<Photoreceptor>* Factory::createPhotoreceptors() {
	srand(time(NULL));

	Quadtree<Photoreceptor>* layer = new Quadtree<Photoreceptor>(Region(Point(0,0), Point(size,size)));
	Photoreceptor::PhotoreceptorType type;
	
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++){
			int typeCheck = rand() % 10;
			if (typeCheck < 6)
				type = Photoreceptor::PhotoreceptorType::RedCone;
			else if (typeCheck < 9)
				type = Photoreceptor::PhotoreceptorType::GreenCone;
			else
				type = Photoreceptor::PhotoreceptorType::BlueCone;

			Point p = Point(-size / 2.0f + ((float)j), -size / 2.0f + ((float)i));
			layer->insert(Data<Photoreceptor>(p, new Photoreceptor(type, p)));
		}
	}

	return layer;
}

//Makes a size x size grid of OpponentProcessors with channel types randomly chosen with the following probabilities
//Luminance: 60%
//RedGreen: 30%
//BlueYellow: 10%
//
//All Field Types are OnCenter

Quadtree<Opponent>* Factory::createOpponents() {
	srand(time(NULL));

	Quadtree<Opponent>* layer = new Quadtree<Opponent>(Region(Point(0, 0), Point(size / 2, size / 2)));;
	Opponent::OpponentChannelType type;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int typeCheck = rand() % 10;
			if (typeCheck < 6)
				type = Opponent::OpponentChannelType::Luminance;
			else if (typeCheck < 9)
				type = Opponent::OpponentChannelType::RedGreen;
			else
				type = Opponent::OpponentChannelType::BlueYellow;

			Point p = Point(-size / 2 + ((float)j), -size / 2 + ((float)i));
			layer->insert(Data<Opponent>(p, new Opponent(type, Opponent::OpponentFieldType::OnCenter, p)));
		}
	}

	return layer;
}

void Factory::connectOpponents(Quadtree<Photoreceptor>& photquad, Quadtree<Opponent>& ops) {
	std::vector<Data<Opponent >> opData = ops.queryRange(ops.getBoundary());
	for (Data<Opponent> op : opData) {
		std::vector<Photoreceptor*>* photCAdd = new std::vector<Photoreceptor*>();
		std::vector<Photoreceptor*>* photSAdd = new std::vector<Photoreceptor*>();
		double rad = op.load->getRange();
		std::vector<Data<Photoreceptor>> photData = photquad.queryRange(Region(op.load->getPoint(), Point(rad,rad)));
		for (Data<Photoreceptor> dp : photData) {
			if (op.load->isCompatible(dp.load)) {
				if ((op.load->getPoint() - dp.load->getPoint()).magnitude() <= rad/2) {
					photCAdd->push_back(dp.load);
				}
				else if ((op.load->getPoint() - dp.load->getPoint()).magnitude() <= rad) {
					photSAdd->push_back(dp.load);
				}
			}
		}
		op.load->setCenterConnections(*photCAdd);
		op.load->setSurroundConnections(*photSAdd);
	}
}