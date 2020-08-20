#ifndef QUADRANT_H
#define QUADRANT_H

#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

namespace dsa {
namespace quadrant {

using zone_t = tuple<double, double, double, double>;

enum quad_enum 
{
	NO_QUADRANT = -1,
	NE,
	NW,
	SE,
	SW
};

quad_enum quad_lookup[5] = {NO_QUADRANT, NE, NW, SE, SW};

string quad_map[5] = {"NO_QUADRANT", "NE", "NW", "SE", "SW"};

quad_enum quadrant_of(point_t p, point_t c) {
	auto [px, py] = p;
	auto [cx, cy] = c;

	if (px > cx)
		if (py > cy)
			return NE;
		else
			return SE;
	else
		if (py > cy)
			return NW;
		else
			return SW;
}

quad_t subdivide_zone(quad_enum t, quad_t q) {
	auto [cx, cy] = q.first;
	auto [xd, yd] = q.second;

	double xf[4] = {1, -1,  1, -1};
	double yf[4] = {1,  1, -1, -1};

	xd *= 0.5; yd *= 0.5;
	cx += xf[t]*xd; cy += yf[t]*yd;

	return { {cx, cy}, {xd, yd} };
}

zone_t get_zone(quad_t q) {
	auto [cx, cy] = q.first;
	auto [xd, yd] = q.second;

	return { cx - xd, cx + xd, cy - yd, cy + yd };
}

bool contains(point_t p, quad_t q) {
	auto [xmin, xmax, ymin, ymax] = get_zone(q);
	auto [px, py] = p;

	if (px >= xmin && px <= xmax && py >= ymin && py <= ymax)
		return true;
	else
		return false;
}

bool contains(quad_t q, quad_t z) {
	auto [qxmin, qxmax, qymin, qymax] = get_zone(q);
	point_t qp[4] = {{qxmin, qymin}, {qxmax, qymin}, {qxmin, qymax}, {qxmax, qymax}};
	for (auto p: qp)
		if (!quadrant::contains(p, z))
			return false;

	return true;
}

// bool intersects(quad_t q, quad_t z) {
// 	auto [qxmin, qxmax, qymin, qymax] = get_zone(q);
// 	point_t qp[4] = {{qxmin, qymin}, {qxmax, qymin}, {qxmin, qymax}, {qxmax, qymax}};
// 	for (auto p: qp)
// 		if (quadrant::contains(p, z))
// 			return true;

// 	return false;
// }

bool intersects(quad_t q, quad_t z) {
	auto [qx, qy] = q.first;
	auto [qxd, qyd] = q.second;
	
	auto [zx, zy] = z.first;
	auto [zxd, zyd] = z.second;

	if (((qxd + zxd) > fabs(qx - zx)) || ((qyd + zyd) > fabs(qy - zy)))
		return true;

	return false;
}

} // namespace quadrant
} // namespace dsa

#endif