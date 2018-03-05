#pragma once
#ifndef _POINT_
#define _POINT_


struct Point {
	double x;
	double y;
	Point() :x(0.0), y(0.0) {}
	Point(double xc, double yc) : x(xc), y(yc) {}
};

#endif