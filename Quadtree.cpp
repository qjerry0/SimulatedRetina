#include "stdafx.h"
#include "Quadtree.h"

template <class T>
Quadtree<T>::Quadtree() {
	CAPACITY = 4;
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
	Point qSize = Point(boundary.halfSize.x, boundary.halfSize.y);
	Point qCenter = Point(boundary.center.x - qSize.x, boundary.center.y - qSize.y);
	nw = new Quadtree(Region(qCenter, qSize));

	qCentre = Point(boundary.center.x + qSize.x, boundary.center.y - qSize.y);
	ne = new Quadtree(AABB(qCenter, qSize));

	qCentre = Point(boundary.center.x - qSize.x, boundary.center.y + qSize.y);
	sw = new Quadtree(AABB(qCenter, qSize));

	qCentre = Point(boundary.center.x + qSize.x, boundary.center.y + qSize.y);
	se = new Quadtree(AABB(qCenter, qSize));
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