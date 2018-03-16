#pragma once
#include "stdafx.h"
#include <vector>
#include "Point.h"
#include "Color.h"

#ifndef PHOTORECEPTOR
#define PHOTORECEPTOR

class Photoreceptor {
protected:
	Point location;
	enum class PhotoreceptorType {RedCone, GreenCone, BlueCone, Rod};
	PhotoreceptorType pType;
	Color input;
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
	void setInput(Color c);

	/**
		Proccesses a given Color input and returns the activation strength

		@param color The color received by the photoreceptor
		@return The strength of the output signal [0-1]
	*/
	float process();


	/**
		Photoreceptor outputs result

		@return The strength of the output signal [0-1]
	*/
	float output();
	
	/**
		The location of the photoreceptor

		@return Location of the photoreceptor
	*/
	virtual Point getPoint();
};

#endif 
