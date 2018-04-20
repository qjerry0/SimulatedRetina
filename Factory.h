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
	static const float size;
	static const float maxDensity;
	static const double maxDistance;

	/**
	Generates all photoreceptors in the model

	@return The photoreceptors generated
	*/
	static Quadtree<Photoreceptor>* createPhotoreceptors();


	/**
	Computes the number of photoreceptors to be generated

	@return The photoreceptors generated
	@param i The width offset of the pixel
	@param j The height offset of the pixel
	*/
	static int Factory::numberOfPhotoreceptors(int i, int j);

	/**
	Generates a photoreceptor in the model

	@return The photoreceptors generated
	@param layer Layer containing the photoreceptors
	@param i The width offset of the pixel
	@param j The height offset of the pixel
	*/
	static void createPhotoreceptor(Quadtree<Photoreceptor>* layer, int i, int j);

	/**
		Generates all opponent processors in the model

		@return The opponent processors generated
	*/
	static Quadtree<Opponent>* createOpponents();

	/**
		Connects all the opponent processors with photoreceptors

		@param p Photoreceptors in the model
		@param o Opponent processors in the model
	*/
	static void connectOpponents(Quadtree<Photoreceptor>& p, Quadtree<Opponent>& o);
};

#endif 