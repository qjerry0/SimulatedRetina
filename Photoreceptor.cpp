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


//Assume Activation Depends only on the Cone Color Value (i.e. Red Cones use Red Color Value,
//Green Cones Use Green Color Value, Blue Cones use Blue)

float Photoreceptor::process(Color color) {
	switch (pType) {
	case PhotoreceptorType::RedCone:
		return color.r;
		output = color.r;
		break;
		
	case PhotoreceptorType::GreenCone:
		return color.g;
		output = color.g;
		break;

	case PhotoreceptorType::BlueCone:
		return color.b;
		output = color.b;
		break;

	default:
		return color.magnitude();
		output = color.magnitude();
		break;
	}
}

