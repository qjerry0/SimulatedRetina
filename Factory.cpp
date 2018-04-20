#include "stdafx.h"
#include <random>
#include <ctime>
#include <iostream>
#include "ppl.h"
#include "Factory.h"

const float Factory::size = 256.0f;
const float Factory::maxDensityPhotoreceptor = 16.0f;
const float Factory::maxDensityGanglion = 4.0f;
const double Factory::maxDistance = sqrt(Factory::size*Factory::size / 4 + Factory::size*Factory::size / 4);

int Factory::numberOfCells(int i, int j, bool isPhotoreceptor) {
		
	int real_i = i - size / 2;
	int real_j = j - size / 2;
	double dist = sqrt(real_i*real_i + real_j*real_j);
	dist = dist > maxDistance ? maxDistance : dist;

	//number of photoreceptors generated is inversely proportional to distance from (0,0)
	if(isPhotoreceptor){
		return maxDensityPhotoreceptor * (1.0 - (dist / maxDistance) / 0.75) + 1.0;
	}
	else {
		return maxDensityGanglion * (1.0 - (dist / maxDistance)) + 2.0;
	}
	
	//std::cout << "num at this cell is = " << num << "\n";
	
}

//Makes a size x size grid of Cones with types randomly chosen with the following probabilities
//Red: 60%
//Green: 30%
//Blue: 10%

Quadtree<Photoreceptor>* Factory::createPhotoreceptors() {
	srand(time(NULL));

	Quadtree<Photoreceptor>* layer = new Quadtree<Photoreceptor>(Region(Point(0,0), Point(size/2,size/2)));
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++){

			//create number of photoreceptors based on density
			int num = numberOfCells(i, j, true);
			for (int k = 0; k < num; k++) {
				createPhotoreceptor(layer, i, j);
			}
		}
	}

	return layer;
}

void Factory::createPhotoreceptor(Quadtree<Photoreceptor>* layer, int i, int j) {
	Photoreceptor::PhotoreceptorType type;
	int typeCheck = rand() % 10;
	double xrand = rand() / double(RAND_MAX);
	double yrand = rand() / double(RAND_MAX);
	if (typeCheck < 6)
		 type = Photoreceptor::PhotoreceptorType::RedCone;
	else if (typeCheck < 9)
		 type = Photoreceptor::PhotoreceptorType::GreenCone;
	else
		 type = Photoreceptor::PhotoreceptorType::BlueCone;
	Point p = Point(-size / 2.0f + ((float)j) + xrand, -size / 2.0f + ((float)i) + yrand);
	layer->insert(Data<Photoreceptor>(p, new Photoreceptor(type, p)));
	
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
			int num = numberOfCells(i, j, false);
			for (int k = 0; k < num; k++) {
				createOpponent(layer, i, j);
			}
		}
	}

	return layer;
}

void Factory::createOpponent(Quadtree<Opponent>* layer, int i, int j) {
	Opponent::OpponentChannelType type;
	int typeCheck = rand() % 10;
	double xrand = rand() / double(RAND_MAX);
	double yrand = rand() / double(RAND_MAX);
	if (typeCheck < 6)
		type = Opponent::OpponentChannelType::Luminance;
	else if (typeCheck < 9)
		type = Opponent::OpponentChannelType::RedGreen;
	else
		type = Opponent::OpponentChannelType::BlueYellow;

	Point p = Point(-size / 2.0f + ((float)j) + xrand, -size / 2.0f + ((float)i) + yrand);
	layer->insert(Data<Opponent>(p, new Opponent(type, Opponent::OpponentFieldType::OnCenter, p)));
}

void Factory::connectOpponents(Quadtree<Photoreceptor>& photquad, Quadtree<Opponent>& ops) {
	std::vector<Data<Opponent >> opData = ops.queryRange(ops.getBoundary());
	int opsize = opData.size();
	int i = 0;
	concurrency::parallel_for_each(opData.begin(), opData.end(), [&](Data<Opponent> op) {
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

			if (((int)((100 * i) / opsize)) > ((int)((100 * (i-1)) / opsize)) && ((int)((100 * i) / opsize)) % 5 == 0) {
				std::cout << "Connections are " << ((int)((100 * i) / opsize)) << "% complete.\n";
			}

			i++;
		}
	);
}