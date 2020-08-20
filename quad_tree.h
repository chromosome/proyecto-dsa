#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <bits/stdc++.h>
#include "utils.h"
#include "node.h"
#include "quadrant.h"

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

	if (t == NO_QUADRANT)
		cout << "quadrant: " << q.first << endl;
	else
		cout << quad_map[t] << " quadrant: " << q.first << endl;
}


class quad_tree
{
	vector<data_t> data;

	size_t size_ = 0;

	const quad_t origin = { {.0, .0}, {90.0, 180.0} };

	node* root = nullptr;

	tuple<node*, quad_t, quad_enum> subdivide(node* n, quad_t q, point_t p) {
		auto t = quadrant_of(p, q.first);
		auto s = subdivide_zone(t, q);

		return { n->get_child(t), s, t };
	}

	bool search(point_t p, function<void(node*)> f = nullptr) {
		if (f == nullptr)
			f = [](node* n){};

		node* current = root;
		quad_t q = origin;

		while ((current != nullptr) && (current->get_color() == node::GREY)) {
			f(current);

			quad_enum t;
			tie(current, q, t) = subdivide(current, q, p);
		}

		if (current != nullptr) {
			f(current);
			return true;
		}
		else
			return false;
	}

	void bfs(function<bool(node*, quad_t)> f) {
		if (root != nullptr && root->get_color() == node::BLACK)
			f(root, origin);

		queue<tuple<node*, quad_t>> Q;
		Q.push({root, origin});

		while(!Q.empty()) {
			auto [n, q] = Q.front();
			Q.pop();

			for (auto& t: quad_lookup) {
				auto c = n->get_child(t);
				auto qs = subdivide_zone(t, q);

				if (f(c, qs))
					Q.push({c, qs});
			}
		}
	}

public:

	quad_tree(vector<data_t> _data)
	: data(_data) 
	{
		// cout << std::setprecision(std::numeric_limits<double>::digits10);
		for (auto& record: data)
			if (insert(&record))
				population += record.second;
	}

	data_t* find(point_t p) {
		node* entry = nullptr;
		auto func = 
			[&entry, p] (node* n) {
				if ((n->get_color() == node::BLACK) && (p == n->get_point()))
					entry = n;
			};
		search(p, func);
		if (entry != nullptr)
			return entry->get_data();
		else
			return nullptr;
	}

	int depth(point_t p) {
		int d = 0;
		auto func = [&d] (node* n) { d++; };
		search(p, func);
		return d;
	}

	long int get_total_population(quad_t z) {
		long int total_population = 0;
		auto func = 
			[&total_population, z] (node* n, quad_t q) { 
				if (n != nullptr
					&& (intersects(q, z) || intersects(z, q)/*contains(z, q)*/)) {

					// y si es nodo negro
					if (n->get_color() == node::BLACK
						&& contains(n->get_point(), z)) {

						// se cuenta su poblacion
						total_population += n->get_data()->second;
					}
					return true;
				}
				return false;
			};
		bfs(func);
		return total_population;
	}

	long int get_total_cities(quad_t z) {
		long int total_cities = 0;
		auto func = 
			[&total_cities, z] (node* n, quad_t q) { 
				if (n != nullptr
					&& (intersects(q, z) || intersects(z, q)/*contains(z, q)*/)) {

					// y si es nodo negro
					if (n->get_color() == node::BLACK
						&& contains(n->get_point(), z)) {

						// se cuenta su poblacion
						total_cities++;
					}
					return true;
				}
				return false;
			};
		bfs(func);
		return total_cities;
	}

	long int get_max_depth() {
		double max_depth = 0;
		auto func = 
			[&max_depth, this] (node* n, quad_t q) { 
				if (n != nullptr) {
					if (n->get_color() == node::BLACK) {
						double depth = log2(get<0>(origin.second)/get<0>(q.second));
						max_depth = max_depth > depth ? max_depth : depth;
						return false;
					}
					return true;
				}
				return false;
			};
		bfs(func);
		return max_depth;
	}

	bool insert(data_t* d) {
		point_t p = d->first;
		quad_t q = origin;
		quad_t qs = origin;
		// cout << "[insert]: " << p << endl;

		node* current = root;
		node* child   = current;

		if (root == nullptr) {
			root = new node(d, node::BLACK);

			// cout << "root: ";
			// debug(root, q, NO_QUADRANT);
			// cout << "[inserting]: " << p << endl;
			// cout << "[done]" << endl << endl;

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

			// cout << "current: ";
			// debug(current, q, NO_QUADRANT);

			// cout << "child: ";
			// debug(child, qs, t);

			// cout << endl;

			// nodo blanco
			if (child == nullptr) {
				child = current->make_child(t, d);
				// cout << "[inserting]: " << p << endl;
			}
			else if (child->get_color() == node::BLACK) {
				if (p == child->get_point())
					return false;

				auto [black, qb, tb] = 
					subdivide(child, qs, child->get_point());

				black = child->make_child(tb);
			}
		}

		// cout << "[done]" << endl << endl;

		size_++;
		return true;
	}

	size_t size() {
		return size_;
	}
};

} // namespace dsa

#endif
