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
	enum class OpponentChannelType {Luminance, RedGreen, BlueYellow};
	enum class OpponentFieldType {OnCenter, OnSurround};
	OpponentChannelType ocType;
	OpponentFieldType ofType;
	std::vector<Photoreceptor*> inputCenterConnectedCells;
	std::vector<Photoreceptor*> inputSurroundConnectedCells;
	float output;

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
		Sets input center connections of the opponent processor

		@param v The Photoreceptors it is connected to
	*/
	void setCenterConnections(std::vector<Photoreceptor*> v);

	/**
	Sets input surround connections of the opponent processor

	@param v The Photoreceptors it is connected to
	*/
	void setSurroundConnections(std::vector<Photoreceptor*> v);
	
	/**
	Opponent processor processes input from connected Photoreceptors
	
	@return The strength of the output signal [0-1]
	*/
	float process();
	
	/**
		Opponent processor outputs result with current connections

		@return The strength of the output signal [0-1]
	*/
	float process();


	/**
		The location of the opponent processor

		@return Location of the opponent processor
	*/
	Point getPoint();

	/**
		The channel type of the opponent processor
		
		@return Opponent Channel Type {Luminance, RedGreen, BlueYellow}
	*/
	OpponentChannelType getChannelType();
	
	/**
		The field processing type of the opponent processor
		
		@return Opponent Field Processing Type {OnCenter, OnSurround}
	*/
	OpponentFieldType getFieldType();
	
	/**
		Resets all connected Photoreceptors
	*/
	void resetConnections();

};

#endif 
