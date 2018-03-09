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
		Proccesses a given Color input and returns an output from [0-1]

		@param color The color received by the photoreceptor
		@return The strength of the photoreceptor's activation
	*/
	float process(Color color);


	/**
		The location of the photoreceptor

		@return Location of the photoreceptor
	*/
	virtual Point getPoint();
};

#endif 
