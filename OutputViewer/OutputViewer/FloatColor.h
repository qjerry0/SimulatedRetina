#pragma once
#include "stdafx.h"
#include <cmath>
#include "SFML\Graphics.hpp"
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

	FloatColor operator+ (const FloatColor& p) {
		return FloatColor(this->r + p.r, this->g + p.g, this->b + p.b);
	}

	FloatColor operator- (const FloatColor& p) {
		return FloatColor(this->r - p.r, this->g - p.g, this->b - p.b);
	}

	FloatColor operator* (const double d) {
		return FloatColor(this->r*d, this->g*d, this->b*d);
	}

	FloatColor operator/ (const double d) {
		return FloatColor(this->r*d, this->g*d, this->b*d);
	}

	static sf::Color toSFMLColor(FloatColor f) {
		return sf::Color(f.r * 255, f.g * 255, f.b * 255);
	}
};

#endif 