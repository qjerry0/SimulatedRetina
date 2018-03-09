#pragma once
#include "stdafx.h"
#include "photoreceptor.h"
#include <vector>
#include "point.h"
#include "color.h"
#ifndef OPPONENT
#define OPPONENT

class Opponent {
protected:
	Point location;
	enum OpponentType {Luminance, RedGreen, BlueYellow};
	std::vector<Photoreceptor*> inputCenterConnectedCells;
	std::vector<Photoreceptor*> inputSurroundConnectedCells;

public:

	/**
		Creates a new opponent processor
	*/
	Opponent();


	/**
		Creates a new opponent processor of the specified type

		@param ot The type of opponent processor
	*/
	Opponent(OpponentType ot);

	/**
		Creates a new opponent processor at the given location

		@param ot The type of opponent processor
		@param p The location of the opponent processor
	*/
	Opponent(OpponentType ot, Point p);

	/**
		Sets input connections of the opponent processor

		@param v The Photoreceptors it is connected to
	*/
	void setConnections(std::vector<Photoreceptor*> v);


	/**
		Opponent processor outputs result

		@return An rgb color
	*/
	Color output();


	/**
		The location of the opponent processor

		@return Location of the opponent processor
	*/
	virtual Point getPoint();

	/**
		Receives input from center connected photoreceptors

		@param v Intensity values of photoreceptors
	*/
	void acquireInputCenter(std::vector<float> v);

	/**
		Receives input from surround connected photoreceptors

		@param v Intensity values of photoreceptors
	*/
	void acquireInputSurround(std::vector<float> v);

	/**
		Resets all connected Photoreceptors
	*/
	void resetConnections();

};

#endif 
