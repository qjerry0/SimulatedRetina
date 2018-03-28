#pragma once
#include "stdafx.h"
#ifndef COLOR
#define COLOR

struct FloatColor {
	double r;
	double g;
	double b;
	FloatColor() :r(0.0), g(0.0), b(0.0) {}
	FloatColor(double rc, double gc, double bc) : r(rc), g(gc), b(bc) {}

	double magnitude() {
		return sqrt(r * r + g * g + b * b);
	}
};

#endif 