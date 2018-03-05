#pragma once
#include "stdafx.h"
#include <vector>
#include "point.h"
#include "opponent.h"
#include "photoreceptor.h"
#ifndef OPPONENTFACTORY
#define OPPONENTFACTORY


class OpponentFactory {

public:
	/**
		Generates all opponent processors in the model

		@return The opponent processors generated
	*/
	std::vector<Opponent*> createOpponents();

	/**
		Connects all the opponent processors with photoreceptors

		@param p Photoreceptors in the model
		@param o Opponent processors in the model
	*/
	static void connectOpponents(std::vector<Photoreceptor*> p, std::vector<Opponent*> o);
};

#endif 