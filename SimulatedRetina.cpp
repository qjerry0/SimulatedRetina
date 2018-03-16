// SimulatedRetina.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Photoreceptor.h"
#include "Opponent.h"
#include "Factory.h"

int main()
{
	std::vector<Photoreceptor*> photoreceptorLayer = Factory::createPhotoreceptors();
	std::vector<Opponent*> opponentLayer = Factory::createOpponents();
	
	Factory::connectOpponents(photoreceptorLayer, opponentLayer);

	for (Photoreceptor* p : photoreceptorLayer) {
		p->process();
	}

	for (Opponent* o : opponentLayer) {
		o->process();
	}
}

