#pragma once
#include "stdafx.h"
#ifndef COLOR
#define COLOR

struct Color {
	double r;
	double g;
	double b;
	Color() :r(0.0), g(0.0), b(0.0) {}
	Color(double rc, double gc, double bc) : r(rc), g(gc), b(bc) {}

	double magnitude() {
		return sqrt(r * r + g * g + b * b);
	}
};

#endif 