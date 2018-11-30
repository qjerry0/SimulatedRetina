#include "stdafx.h"
#include "Point.h"
#include "Data.h"
#include <vector>

#ifndef QUADTREE
#define QUADTREE

struct Region {
	Point center;
	Point halfSize;

	Region(Point center = Point(), Point halfSize = Point()) : center(center), halfSize(halfSize) {};

	bool contains(Point a)
	{
		if (a.x < center.x + halfSize.x && a.x >= center.x - halfSize.x)
		{
			if (a.y < center.y + halfSize.y && a.y >= center.y - halfSize.y)
			{
				return true;
			}
		}
		return false;
	}

	bool intersects(Region other)
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

template <class T>
Quadtree<T>::Quadtree() {
	CAPACITY = 2;
	nw = nullptr;
	ne = nullptr;
	sw = nullptr;
	se = nullptr;
	boundary = Region();
	objects = std::vector< Data<T> >();
}

template <class T>
Quadtree<T>::Quadtree(Region boundary)
{
	objects = std::vector< Data<T> >();
	CAPACITY = 4;
	nw = nullptr;
	ne = nullptr;
	sw = nullptr;
	se = nullptr;
	this->boundary = boundary;
}

template <class T>
Quadtree<T>::~Quadtree()
{
	delete nw;
	delete sw;
	delete ne;
	delete se;
}

template <class T>
Region Quadtree<T>::getBoundary()
{
	return boundary;
}

template <class T>
void Quadtree<T>::subdivide()
{
	Point qSize = Point(boundary.halfSize.x/2, boundary.halfSize.y/2);
	Point qCenter = Point(boundary.center.x - qSize.x, boundary.center.y + qSize.y);
	nw = new Quadtree(Region(qCenter, qSize));

	qCenter = Point(boundary.center.x + qSize.x, boundary.center.y + qSize.y);
	ne = new Quadtree(Region(qCenter, qSize));

	qCenter = Point(boundary.center.x - qSize.x, boundary.center.y - qSize.y);
	sw = new Quadtree(Region(qCenter, qSize));

	qCenter = Point(boundary.center.x + qSize.x, boundary.center.y - qSize.y);
	se = new Quadtree(Region(qCenter, qSize));
}

template <class T>
bool Quadtree<T>::insert(Data<T> d)
{
	if (!boundary.contains(d.pos))
	{
		return false;
	}

	if (objects.size() < CAPACITY)
	{
		objects.push_back(d);
		return true;
	}

	if (nw == nullptr)
	{
		subdivide();
	}

	if (nw->insert(d))
	{
		return true;
	}
	if (ne->insert(d))
	{
		return true;
	}
	if (sw->insert(d))
	{
		return true;
	}
	if (se->insert(d))
	{
		return true;
	}

	return false;
}

template <class T>
std::vector<Data<T>> Quadtree<T>::queryRange(Region range)
{
	std::vector< Data<T> > pInRange = std::vector< Data<T> >();

	if (!boundary.intersects(range))
	{
		return pInRange;
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (range.contains(objects.at(i).pos))
		{
			pInRange.push_back(objects.at(i));
		}
	}

	if (nw == nullptr)
	{
		return pInRange;
	}

	std::vector< Data<T> > temp = nw->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = ne->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = sw->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = se->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	return pInRange;
}

#endif // !QUADTREE
