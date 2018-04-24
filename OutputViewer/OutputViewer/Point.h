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

	Point operator+ (const Point& p) {
		return Point(this->x + p.x, this->y + p.y);
	}

	Point operator- (const Point& p) {
		return Point(this->x - p.x, this->y - p.y);
	}

	Point operator* (const Point p) {
		return Point(this->x*p.x, this->y*p.y);
	}

	Point operator/ (const Point p) {
		return Point(this->x / p.x, this->y / p.y);
	}

	Point operator* (const double d) {
		return Point(this->x*d, this->y*d);
	}

	Point operator/ (const double d) {
		return Point(this->x/d, this->y/d);
	}
};

#endif