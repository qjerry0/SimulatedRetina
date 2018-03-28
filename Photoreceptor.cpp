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

void Photoreceptor::setInput(sf::Color c) {
	input = FloatColor((float)(c.r)/255.0f, (float)(c.g)/255.0f, (float)(c.b)/255.0f);
}


//Assume Activation Depends only on the Cone Color Value (i.e. Red Cones use Red Color Value,
//Green Cones Use Green Color Value, Blue Cones use Blue)

double Photoreceptor::process() {
	switch (pType) {
	case PhotoreceptorType::RedCone:
		output = input.r;
		return input.r;
		
	case PhotoreceptorType::GreenCone:
		output = input.g;
		return input.g;

	case PhotoreceptorType::BlueCone:
		output = input.b;
		return input.b;

	default:
		output = sqrt(input.r*input.r + input.g*input.g + input.b*input.b);
		return sqrt(input.r*input.r + input.g*input.g + input.b*input.b);
	}
}

double Photoreceptor::getOutput() {
	return output;
}

