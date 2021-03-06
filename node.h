#ifndef NODE_H
#define NODE_H

#include <bits/stdc++.h>
#include "utils.h"
#include "quadrant.h"

namespace dsa {

using zone_t = tuple<double, double, double, double>;

struct node {
	enum color_enum {
		GREY,
		BLACK
	};

	data_t data = {};
	node* child[4] = {nullptr, nullptr, nullptr, nullptr};
	node* father = nullptr;
	color_enum color = GREY;

	node(data_t _data = {}, color_enum _color = GREY)
	: data  (_data  )
	, color (_color )
	{}

	~node() {}

	node* get_child(quadrant::quad_enum t) {
		return child[t];
	}

	node* make_child(quadrant::quad_enum t, data_t d) {
		child[t] = new node(d, BLACK);
        child[t]->set_father(this);
		return child[t];
	}

	node* make_child(quadrant::quad_enum t) {
		auto child = make_child(t, data);
		make_grey();

		return child;
	}

	void set_father(node* n) {
        father = n;
	}

	node* get_father(void) {
        return father;
	}

	void make_grey() {
		clear_data();
		set_color(GREY);
	}

	void set_data(data_t d) {
		data = d;
	}

	data_t get_data() {
		return data;
	}

	void clear_data() {
		data = {};
	}

	const point_t& get_point() {
		return data.first;
	}

	const unsigned& get_value() {
		return data.second;
	}

	color_enum get_color() {
		return color;
	}

	void set_color(node::color_enum c) {
		color = c;
	}
};

}

#endif
