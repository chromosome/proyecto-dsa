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
	node* child[4] = {nullptr, nullptr, nullptr, nullptr};
	color_enum color = GREY;

	node(data_t* _data = nullptr, color_enum _color = GREY)
	: data  (_data  )
	, color (_color )
	{}
	
	~node() { 
		if (data != nullptr)
			delete data; 
	}

	node* get_child(quadrant::quad_enum t) {
		return child[t];
	}

	node* make_child(quadrant::quad_enum t, data_t* d) {
		child[t] = new node(d, BLACK);

		return child[t];
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

	color_enum get_color() {
		return color;
	}

	void set_color(node::color_enum c) {
		color = c;
	}
};

}

#endif
