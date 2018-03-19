#include "stdafx.h"
#include "Photoreceptor.h"

Photoreceptor::Photoreceptor() {

}

Photoreceptor::Photoreceptor(PhotoreceptorType t) {
	pType = t;
}

Photoreceptor::Photoreceptor(PhotoreceptorType t, Point p) {
	pType = t;
	location = p;
}

Photoreceptor::PhotoreceptorType Photoreceptor::getType() {
	return pType;
}

Point Photoreceptor::getPoint() {
	return location;
}

void Photoreceptor::setInput(Color c) {
	input = c;
}


//Assume Activation Depends only on the Cone Color Value (i.e. Red Cones use Red Color Value,
//Green Cones Use Green Color Value, Blue Cones use Blue)

double Photoreceptor::process() {
	switch (pType) {
	case PhotoreceptorType::RedCone:
		return input.r;
		
	case PhotoreceptorType::GreenCone:
		return input.g;

	case PhotoreceptorType::BlueCone:
		return input.b;

	default:
		return input.magnitude();
	}
}

double Photoreceptor::getOutput() {
	return output;
}

