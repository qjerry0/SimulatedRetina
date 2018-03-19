#include "stdafx.h"
#include "Point.h"
#include <vector>

#ifndef QUADTREE
#define QUADTREE

struct Region {
	Point center;
	Point halfSize;

	Region(Point center = Point(), Point halfSize = Point()) : center(center), halfSize(halfSize) {};

	bool contains(const Point a) const
	{
		if (a.x < center.x + halfSize.x && a.x > center.x - halfSize.x)
		{
			if (a.y < center.y + halfSize.y && a.y > center.y - halfSize.y)
			{
				return true;
			}
		}
		return false;
	}

	bool intersects(const Region other) const
	{
		//this right > that left                                          this left <s that right
		if (center.x + halfSize.x > other.center.x - other.halfSize.x || center.x - halfSize.x < other.center.x + other.halfSize.x)
		{
			// This bottom > that top
			if (center.y + halfSize.y > other.center.y - other.halfSize.y || center.y - halfSize.y < other.center.y + other.halfSize.y)
			{
				return true;
			}
		}
		return false;
	}
};

template <typename T>
struct Data
{
	Point pos;
	T* load;

	Data(Point pos = Point(), T* data = nullptr) : pos(pos), load(data) {};
};

template <class T>
class Quadtree
{
private:
	//4 children
	Quadtree * nw;
	Quadtree* ne;
	Quadtree* sw;
	Quadtree* se;

	Region boundary;

	std::vector< Data<T> > objects;

	int CAPACITY;
public:
	Quadtree<T>();
	Quadtree<T>(Region boundary);

	~Quadtree();

	Region getBoundary();
	bool insert(Data<T> d);
	void subdivide();
	std::vector< Data<T> > queryRange(Region range);
};

#endif // !QUADTREE
