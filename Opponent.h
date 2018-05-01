#pragma once
#include "stdafx.h"
#include <vector>
#include "Photoreceptor.h"
#include "Point.h"
#include "FloatColor.h"

#ifndef OPPONENT
#define OPPONENT

class Opponent {
public:
	enum class OpponentChannelType { Luminance, RedGreen, BlueYellow };
	enum class OpponentFieldType { OnCenter, OnSurround };

protected:
	static const float SURROUND_CONSTANT;
	static const float OPPONENT_MAX_FIELD_SIZE;
	static const float OPPONENT_MIN_FIELD_SIZE;
	Point location;
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
	Opponent(OpponentChannelType ot, OpponentFieldType oft);

	/**
		Creates a new opponent processor at the given location

		@param ot The type of opponent processor
		@param p The location of the opponent processor
	*/
	Opponent(OpponentChannelType ot, OpponentFieldType oft, Point p);

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
	double process();

	/**
		Opponent outputs result

		@return The strength of the output signal [(-1)-1]
	*/
	double getOutput();


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
		The radius of the opponent processor's receptive field
	
		@return Radius of Receptive Field
	*/
	double getRange(float maxDist);

	/**
		Checks whether the photoreceptor should be processed by the opponent


		@return True if the photoreceptor can be processed, false if not
	*/
	bool isCompatible(Photoreceptor* p);

	/**
		Resets all connected Photoreceptors
	*/
	void resetConnections();


	std::string toString();
};

#endif 
