#pragma once
#include "stdafx.h"
#include <vector>
#include "Point.h"
#include "FloatColor.h"
#include "SFML\Graphics.hpp"

#ifndef PHOTORECEPTOR
#define PHOTORECEPTOR

class Photoreceptor {
public:
	enum class PhotoreceptorType { RedCone, GreenCone, BlueCone, Rod };

protected:
	Point location;
	PhotoreceptorType pType;
	FloatColor input;
	float output;

public:

	/**
		Returns the type of photoreceptor

		@return type of this photoeceptor
	*/
	PhotoreceptorType getType();

	/**
		Creates a new photoreceptor
	*/
	Photoreceptor();

	/**
		Creates a new photoreceptor of the specified type

		@param pt The type of photoreceptor
	*/
	Photoreceptor(PhotoreceptorType pt);


	/**
		Creates a new photoreceptor of the specified type
		and at the given location

		@param pt The type of photoreceptor
		@param p The location of the photoreceptor
	*/
	Photoreceptor(PhotoreceptorType pt, Point p);

	/**
		Photoreceptor receives light input

		@param c The color received by the photoreceptor
	*/
	void setInput(sf::Color c);

	/**
		Proccesses a given Color input and returns an output from [0,1]

		@param color The color received by the photoreceptor
		@return The strength of the photoreceptor's activation in [0,1]
	*/
	double process();


	/**
		Photoreceptor outputs result

		@return The strength of the output signal [0-1]
	*/
	double getOutput();
	
	/**
		The location of the photoreceptor

		@return Location of the photoreceptor
	*/
	virtual Point getPoint();
};

#endif 
