#include "stdafx.h"
#include <random>
#include <ctime>
#include "Factory.h"

//Makes a size x size grid of Cones with types randomly chosen with the following probabilities
//Red: 60%
//Green: 30%
//Blue: 10%

Quadtree<Photoreceptor> Factory::createPhotoreceptors() {
	srand(time(NULL));
	Quadtree<Photoreceptor> layer;
	Photoreceptor::PhotoreceptorType type;
	int size = 10;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++){
			int typeCheck = rand() % 10;
			if (typeCheck < 6)
				type = Photoreceptor::PhotoreceptorType::RedCone;
			else if (typeCheck < 9)
				type = Photoreceptor::PhotoreceptorType::GreenCone;
			else
				type = Photoreceptor::PhotoreceptorType::BlueCone;

			Point p = Point(-size / 2 + j, -size / 2 + i);
			layer.insert(Data<Photoreceptor>(p, new Photoreceptor(type, p)));
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

Quadtree<Opponent> Factory::createOpponents() {
	srand(time(NULL));
	Quadtree<Opponent> layer;
	Opponent::OpponentChannelType type;
	int size = 10;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int typeCheck = rand() % 10;
			if (typeCheck < 6)
				type = Opponent::OpponentChannelType::Luminance;
			else if (typeCheck < 9)
				type = Opponent::OpponentChannelType::RedGreen;
			else
				type = Opponent::OpponentChannelType::BlueYellow;

			Point p = Point(-size / 2 + j, -size / 2 + i);
			layer.insert(Data<Opponent>(p, new Opponent(type, Opponent::OpponentFieldType::OnCenter, p)));
		}
	}

	return layer;
}

void Factory::connectOpponents(Quadtree<Photoreceptor> phots, Quadtree<Opponent> ops) {
	
}