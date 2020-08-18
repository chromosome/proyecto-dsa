#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <bits/stdc++.h>
#include "node.h"
#include "../utils.h"

using namespace std;

namespace dsa {

class quad_tree
{
	vector<data_t> data;

	node* subdivide(node* n) {
		auto q = n->get_quadrant_for(n->get_point());
		cout << quad_map[q] << endl;

		node* child = n->make_child(q, n->get_data());
		n->make_grey();

		cout << "subdivide: " << child->get_point() << endl;
		cout << "quadrant: " << child->get_center() << endl;
		cin.get();

		return child;
	}

	const double lat =  90.0;
	const double lon = 180.0;
	size_t size_ = 0;

	node* root = nullptr;

	string quad_map[4] = {"NE", "NW", "SE", "SW"};

public:

	quad_tree(vector<data_t> _data)
	: data(_data) 
	{
		// cout << std::setprecision(std::numeric_limits<double>::digits10);
		for (auto& record: data) 
			insert(&record);
	}

	bool insert(data_t* d) {
		point_t p = d->first;
		cout << "[insert]: " << p << endl;

		node* current = root;
		node* child   = nullptr;

		if (root == nullptr) {
			root = new node({.0, .0}, lat, lon, d, node::BLACK);
			cout << "root: " << root->get_point() << endl;
			cout << "quadrant: " << root->get_center() << endl;
			cout << "[done]" << endl << endl;
			size_++;
			return true;
		}

		if (current->get_color() == node::BLACK) {
			current->get_color();

			if (p == current->get_point())
				return false;
			else 
				subdivide(current);
		}

		child = current;

		while((child != nullptr) && (child->get_color() == node::GREY)) {
			current = child;
			auto q = current->get_quadrant_for(p);
			child = current->get_child(q);
			cout << quad_map[q] << endl;

			// nodo blanco
			if (child == nullptr) {
				child = current->make_child(q, d);

				cout << "white: " << child->get_point() << endl;
				cout << "quadrant: " << child->get_center() << endl;
				cin.get();
			}
			else if (child->get_color() == node::BLACK) {
				if (p == child->get_point())
					return false;

				cout << "black: " <<  child->get_point() << endl;
				cout << "quadrant: " << child->get_center() << endl;

				subdivide(child);
			}
		}

		cout << "[done]" << endl << endl;
		size_++;
		return true;
	}

	data_t* find(point_t p) {
		node* current = root;

		if (p == current->get_point())
			return current->get_data();

		while ((current != nullptr) && (current->get_color() == node::GREY)) {
			current = current->get_child(current->get_quadrant_for(p));

			if ((p == current->get_point()) && (current->get_color() == node::BLACK))
				return current->get_data();
		}
		return nullptr;
	}

	size_t size() {
		return size_;
	}
};
	
}

#endif
