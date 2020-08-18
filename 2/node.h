#ifndef NODE_H
#define NODE_H

#include <bits/stdc++.h>
#include "../utils.h"
#include "quadrant.h"

namespace dsa {

using zone_t = tuple<double, double, double, double>;

struct node {
	enum color_enum {
		GREY,
		BLACK
	};

	data_t* data = nullptr;
	quadrant quad;
	node* children[4] = {nullptr, nullptr, nullptr, nullptr};
	color_enum color = GREY;

	node(point_t _center, 
		 double _xdist, 
		 double _ydist, 
		 data_t* _data = nullptr, 
		 color_enum _color = GREY)
	: data  (_data  )
	, color (_color )
	, quad  (_center, _xdist, _ydist)
	{}

	node(quadrant _quad) 
	: quad(_quad)
	{
		// quad 		= other_p->quad;
	}
	
	~node() { 
		if (data != nullptr)
			delete data; 
	}

	node* get_child(quadrant::quad_enum q) {
		return children[q];
	}

	node* make_child(quadrant::quad_enum q, data_t* d) {
		children[q] = new node(quad);
		children[q]->subdivide_quad(q);
		children[q]->set_color(BLACK);
		children[q]->set_data(d);

		return children[q];
	}

	void make_grey() {
		clear_data();
		set_color(GREY);
	}

	void set_data(data_t* d) {
		data = d;
	}

	void clear_data() {
		data = nullptr;
	}

	data_t* get_data() {
		return data;
	}

	const point_t& get_point() {
		return data->first;
	}

	const point_t& get_center() {
		return quad.center;
	}

	color_enum get_color() {
		return color;
	}

	void set_color(node::color_enum c) {
		color = c;
	}

	quadrant::quad_enum get_quadrant_for(point_t p) {
		return quad.get_quadrant_for(p);
	}

	void subdivide_quad(quadrant::quad_enum q) {
		quad.subdivide_zone(q);
	}

	bool quad_contains(point_t p) {
		return quad.contains(p);
	}
};

}

#endif
