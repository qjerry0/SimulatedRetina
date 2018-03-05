#pragma once
#include "stdafx.h"
#include <vector>
#include "point.h"
#ifndef PHOTORECEPTOR
#define PHOTORECEPTOR

class Photoreceptor {
protected:
	double cellPotential;
	double cellMin;
	double cellMax;
	bool isGraded;
	Point location;
	enum class PhotoreceptorType {Cone, Rod};

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


	void setConnections();


	/**
		Photoreceptor receives light input

		@param i An intensity value in [0, 1]
	*/
	void input(float i);

	/**
		Photoreceptor outputs result

		@return An intensity value in [0, 1] 
	*/
	float output();


	/**
		The location of the photoreceptor

		@return Location of the photoreceptor
	*/
	virtual Point getPoint();


	std::vector<std::vector<float>> acquireInputs();
};

#endif 
