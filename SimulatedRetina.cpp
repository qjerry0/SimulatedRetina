// SimulatedRetina.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Photoreceptor.h"
#include "Opponent.h"
#include "Factory.h"
#include "Quadtree.h"

int main()
{
	Quadtree<Photoreceptor> photoreceptorLayer = Factory::createPhotoreceptors();
	Quadtree<Opponent> opponentLayer = Factory::createOpponents();
	
	Factory::connectOpponents(photoreceptorLayer, opponentLayer);

	for (Data<Photoreceptor> dp : photoreceptorLayer.queryRange(photoreceptorLayer.getBoundary())) {
		(dp.load)->process();
	}

	for (Data<Opponent> dop : opponentLayer.queryRange(opponentLayer.getBoundary())) {
		dop.load->process();
	}
}

