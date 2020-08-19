#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"

using namespace std;

namespace dsa {

using namespace quadrant;

void debug(node* n, quad_t q, quad_enum t = NO_QUADRANT) {
	if (n == nullptr)
		cout << "white node" << endl;
	else 
		if (n->get_color() == node::GREY)
			cout << "grey node" << endl;
		else
			cout << "black node" << endl;

	if (t == NO_QUADRANT) {
		cout << "quadrant: " << q.first << endl;
	}
	else
		cout << quad_map[t] << " quadrant: " << q.first << endl;
}


class quad_tree
{
	vector<data_t> data;

	size_t size_ = 0;

	const quad_t quad = { {.0, .0}, {90.0, 180.0} };

	node* root = nullptr;

	tuple<node*, quad_t, quad_enum> subdivide(node* n, quad_t q, point_t p) {
		auto t = quadrant_of(p, q.first);
		auto s = subdivide_zone(t, q);

		return { n->get_child(t), s, t };
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

			for (auto& t: quad_lookup) {
				auto c = n->get_child(t);
				if (c != nullptr) {
					auto qs = subdivide_zone(t, q);
					if (intersects(qs, z)) {
						if (c->get_color() == node::BLACK
							&& contains(c->get_point(), z)) {
							cout << "aqui!" << endl;
							f(c);
						}
						else
							Q.push({n->get_child(t), qs});
					}
					else if (c->get_color() == node::BLACK
							 && contains(c->get_point(), z)) {
						cout << "aqui2!" << endl;
						f(c);
					}
				}
			}
		}
	}

	data_t* search(point_t p) {
		node* current = root;
		quad_t q = quad;

		while ((current != nullptr) && current->get_color() == node::GREY) {
			quad_enum t;
			tie(current, q, t) = subdivide(current, q, p);
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

			quad_enum t;
			tie(current, q, t) = subdivide(current, q, p);
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
		node* child   = current;

		if (root == nullptr) {
			root = new node(d, node::BLACK);

			cout << "root: ";
			debug(root, q, NO_QUADRANT);
			cout << "[inserting]: " << p << endl;
			cout << "[done]" << endl << endl;

			size_++;
			return true;
		}

		if (current->get_color() == node::BLACK) {
			current->get_color();

			if (p == current->get_point())
				return false;
			else {
				auto [black, qb, tb] 
					= subdivide(current, q, current->get_point());

				black = current->make_child(tb);
			}
		}


		child = current;

		while((child != nullptr) && (child->get_color() == node::GREY)) {
			current = child;
			q = qs;

			quad_enum t;
			tie(child, qs, t) = subdivide(current, q, p);

			cout << "current: ";
			debug(current, q, NO_QUADRANT);

			cout << "child: ";
			debug(child, qs, t);

			cout << endl;

			// nodo blanco
			if (child == nullptr) {
				child = current->make_child(t, d);
				cout << "[inserting]: " << p << endl;
			}
			else if (child->get_color() == node::BLACK) {
				if (p == child->get_point())
					return false;

				auto [black, qb, tb] = 
					subdivide(child, qs, child->get_point());

				black = child->make_child(tb);
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

} // namespace dsa

#endif
