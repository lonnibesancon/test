#pragma once
#include "TouchPoint.h"
#include <vector>
#include <tuple>

class TouchRenderer
{
public:
	TouchRenderer(void);
	~TouchRenderer(void);

	std::vector<TouchPoint> touchpoints ;

	void add(long id, double x, double y);
	void remove(long id);
	void update(long id, double x, double y);

private:
	std::vector<std::tuple<int,TouchPoint>> history;

};

