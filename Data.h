#ifndef DATA
#define DATA

template <typename T>
struct Data
{
	Point pos;
	T* load;

	Data(Point pos = Point(), T* data = nullptr) : pos(pos), load(data) {};
};

#endif // !DATA
