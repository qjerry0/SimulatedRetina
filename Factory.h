#pragma once
#include "stdafx.h"
#include <vector>
#include "Point.h"
#include "Opponent.h"
#include "Photoreceptor.h"
#include "Quadtree.h"

#ifndef FACTORY
#define FACTORY


class Factory {

public:

	/**
	Generates all photoreceptors in the model

	@return The photoreceptors generated
	*/
	static Quadtree<Photoreceptor> createPhotoreceptors();

	/**
		Generates all opponent processors in the model

		@return The opponent processors generated
	*/
	static Quadtree<Opponent> createOpponents();

	/**
		Connects all the opponent processors with photoreceptors

		@param p Photoreceptors in the model
		@param o Opponent processors in the model
	*/
	static void connectOpponents(Quadtree<Photoreceptor> p, Quadtree<Opponent> o);
};

#endif 