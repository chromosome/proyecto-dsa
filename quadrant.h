#ifndef QUADRANT
#define QUADRANT

#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

namespace dsa{

using zone_t = tuple<double, double, double, double>;

struct quadrant {

	enum quad_enum 
	{
		NE,
		NW,
		SE,
		SW
	};

	point_t center;
	double xdist;
	double ydist;

	quadrant() {}

	quadrant(point_t _center, double _xdist, double _ydist)
	: center(_center)
	, xdist (_xdist )
	, ydist (_ydist )
	{}

	void subdivide_zone(quad_enum q) {
		auto [cx, cy] = center;

		double xf[4] = {0.5, -0.5,  0.5, -0.5};
		double yf[4] = {0.5,  0.5, -0.5, -0.5};

		xdist *= xf[q]; ydist *= yf[q];

		center = {cx + xdist, cy + ydist};
	}

	zone_t get_zone() {
		auto [cx, cy] = center;
		return { cx - xdist, cy + xdist, cy - ydist, cy + ydist };
	}

	quad_enum get_quadrant_for(point_t p) {
		auto [cx, cy] = center;
		auto [px, py] = p;

		if (px >= cx)
			if (py >= cy)
				return NE;
			else
				return SE;
		else
			if (py >= cy)
				return NW;
			else
				return SW;
	}

	bool contains(point_t p) {
		auto [xmin, xmax, ymin, ymax] = get_zone();
		auto [px, py] = p;

		if (px > xmin && px < xmax && py > ymin && py < ymax)
			return true;
		else
			return false;
	}


};

}

#endif
