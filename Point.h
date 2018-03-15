#pragma once
#ifndef _POINT_
#define _POINT_

#include <cmath>

struct Point {
	double x;
	double y;
	Point() :x(0.0), y(0.0) {}
	Point(double xc, double yc) : x(xc), y(yc) {}

	double magnitude() {
		return sqrt(x * x + y * y);
	}
};

#endif