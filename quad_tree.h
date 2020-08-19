#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"

using namespace std;

namespace dsa {

class quad_tree
{
	vector<data_t> data;

	size_t size_ = 0;

	const quad_t quad = { {.0, .0}, {90.0, 180.0} };

	node* root = nullptr;

	quad_t subdivide(node* n, quad_t q) {
		auto t = quadrant::quadrant_of(n->get_point(), q.first);
		auto s = quadrant::subdivide_zone(t, q);
		cout << quadrant::quad_map[t] << endl;

		node* child = n->make_child(t, n->get_data());
		n->make_grey();

		cout << "subdivide: " << child->get_point() << endl;
		cout << "quadrant: " << s.first << endl;
		// cin.get();

		return s;
	}

public:

	quad_tree(vector<data_t> _data)
	: data(_data) 
	{
		// cout << std::setprecision(std::numeric_limits<double>::digits10);
		for (auto& record: data) 
			insert(&record);
	}

	void bfs(quad_t z, function<void(node*)> f) {
		if (root != nullptr && root->get_color() == node::BLACK)
			f(root);

		queue<tuple<node*, quad_t>> Q;
		Q.push({root, quad});

		while(!Q.empty()) {
			auto [n, q] = Q.front();
			Q.pop();

			for (auto& t: quadrant::quad_lookup) {
				auto c = n->get_child(t);
				if (c != nullptr) {
					auto qs = quadrant::subdivide_zone(t, q);
					if (quadrant::intersects(qs, z)) {
						if (c->get_color() == node::BLACK
							&& quadrant::contains(c->get_point(), z)) {
							cout << "aqui!" << endl;
							f(c);
						}
						else
							Q.push({n->get_child(t), qs});
					}
					else if (c->get_color() == node::BLACK
							 && quadrant::contains(c->get_point(), z)) {
						cout << "aqui2!" << endl;
						f(c);
					}
				}
			}
		}
	}

	// template<func_t>
	data_t* search(point_t p) {
		node* current = root;
		quad_t q = quad;

		while ((current != nullptr) && current->get_color() == node::GREY) {
			auto t = quadrant::quadrant_of(p, q.first);
			q = quadrant::subdivide_zone(t, q);
			current = current->get_child(t);
		}

		if (current != nullptr && p == current->get_point())
			return current->get_data();

		return nullptr;
	}

	void search(point_t p, function<void(node*)> f) {
		node* current = root;
		quad_t q = quad;

		while ((current != nullptr) && current->get_color() == node::GREY) {
			f(current);
			auto t = quadrant::quadrant_of(p, q.first);
			q = quadrant::subdivide_zone(t, q);
			current = current->get_child(t);
		}

		if (current != nullptr)
			f(current);
	}

	int region_search(point_t p) {
		return 0;
	}

	bool insert(data_t* d) {
		point_t p = d->first;
		quad_t q = quad;
		quad_t qs = quad;
		cout << "[insert]: " << p << endl;

		node* current = root;
		node* child   = nullptr;

		if (root == nullptr) {
			root = new node(d, node::BLACK);
			cout << "root: " << root->get_point() << endl;
			cout << "quadrant: " << q.first << endl;
			cout << "[done]" << endl << endl;
			size_++;
			return true;
		}

		if (current->get_color() == node::BLACK) {
			current->get_color();

			if (p == current->get_point())
				return false;
			else 
				subdivide(current, q);
		}

		child = current;

		while((child != nullptr) && (child->get_color() == node::GREY)) {
			current = child;
			q = qs;

			auto t = quadrant::quadrant_of(p, q.first);
			qs = quadrant::subdivide_zone(t, q);

			child = current->get_child(t);

			cout << quadrant::quad_map[t] << endl;

			// nodo blanco
			if (child == nullptr) {
				child = current->make_child(t, d);

				cout << "white: " << child->get_point() << endl;
				cout << "quadrant: " << qs.first << endl;
				// cin.get();
			}
			else if (child->get_color() == node::BLACK) {
				if (p == child->get_point())
					return false;

				cout << "black: " <<  child->get_point() << endl;
				cout << "quadrant: " << qs.first << endl;

				subdivide(child, qs);
			}
		}

		cout << "[done]" << endl << endl;
		size_++;
		return true;
	}

	size_t size() {
		return size_;
	}
};
	
}

#endif
