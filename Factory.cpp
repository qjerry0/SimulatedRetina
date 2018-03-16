#include "stdafx.h"
#include <random>
#include <ctime>
#include "Factory.h"

//Makes a size x size grid of Cones with types randomly chosen with the following probabilities
//Red: 60%
//Green: 30%
//Blue: 10%

std::vector<Photoreceptor*> Factory::createPhotoreceptors() {
	srand(time(NULL));
	std::vector<Photoreceptor*> layer;
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

			layer.push_back(new Photoreceptor(type, Point(-size/2+j,-size/2+i)));
		}
	}
}

//Makes a size x size grid of OpponentProcessors with channel types randomly chosen with the following probabilities
//Luminance: 60%
//RedGreen: 30%
//BlueYellow: 10%
//
//All Field Types are OnCenter

std::vector<Opponent*> Factory::createOpponents() {
	srand(time(NULL));
	std::vector<Opponent*> layer;
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

			layer.push_back(new Opponent(type, Opponent::OpponentFieldType::OnCenter, Point(-size / 2 + j, -size / 2 + i)));
		}
	}
}

void Factory::connectOpponents(std::vector<Photoreceptor*> phots, std::vector<Opponent*> ops) {
	for (Opponent* o : ops) {

	}
}