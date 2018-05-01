#pragma once
#include "stdafx.h"
#include <vector>
#include "Point.h"
#include "Opponent.h"
#include "Photoreceptor.h"
#include "Quadtree.h"
#include "Data.h"

#ifndef FACTORY
#define FACTORY


class Factory {

public:
	static const float size;
	static const float maxDensityPhotoreceptor;
	static const float minDensityPhotoreceptor;
	static const float spreadParameterPhotoreceptor;
	static const float maxDensityOpponent;
	static const float minDensityOpponent;
	static const float spreadParameterOpponent;
	static const double maxDistance;
	static const float percentRetinaModeled;

	/**
	Generates all photoreceptors in the model

	@return The photoreceptors generated
	*/
	static Quadtree<Photoreceptor>* createPhotoreceptors();

	static int Factory::numberOfCells(int i, int j, bool isPhotoreceptor);

	static void createPhotoreceptor(Quadtree<Photoreceptor>* layer, int i, int j);


	/**
		Generates all opponent processors in the model

		@return The opponent processors generated
	*/
	static Quadtree<Opponent>* createOpponents();

	static void createOpponent(Quadtree<Opponent>* layer, int i, int j);

	/**
		Connects all the opponent processors with photoreceptors

		@param p Photoreceptors in the model
		@param o Opponent processors in the model
	*/
	static void connectOpponents(Quadtree<Photoreceptor>& p, Quadtree<Opponent>& o);
};

#endif 