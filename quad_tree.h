#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <bits/stdc++.h>
#include "utils.h"
#include "node.h"
#include "quadrant.h"

using namespace std;

namespace dsa {

using namespace quadrant;

void debug(node* n, quad_t q, quad_enum t = ORIGIN) {
	if (n == nullptr)
		cout << "white node" << endl;
	else
		if (n->get_color() == node::GREY)
			cout << "grey node" << endl;
		else
			cout << "black node" << endl;

	if (t == ORIGIN)
		cout << "quadrant: " << q.first << endl;
	else
		cout << quad_map[t] << " quadrant: " << q.first << endl;
}


class quad_tree
{
	list<data_t> data;

	size_t size_ = 0;

	const quad_t origin = { {.0, .0}, {90.0, 180.0} };

	node* root = nullptr;

	/* Subdivide --------------------------------------------------------------
	 */
	tuple<node*, quad_t, quad_enum> subdivide(node* n, quad_t q, point_t p) {
		auto t = quadrant_of(p, q.first);
		auto s = subdivide_zone(t, q);

		return { n->get_child(t), s, t };
	}

	/*  Search ----------------------------------------------------------------
	 */
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

	/*  Search ----------------------------------------------------------------
	 */
	bool search(point_t p, function<void(node*, quad_enum)> f = nullptr) {
		if (f == nullptr)
			f = [](node* n, quad_enum t){};

		node* current = root;
		quad_t q = origin;
		quad_enum t = ORIGIN;

		while ((current != nullptr) && (current->get_color() == node::GREY)) {
			f(current, t);

			tie(current, q, t) = subdivide(current, q, p);
		}

		if (current != nullptr) {
			f(current, t);
			return true;
		}
		else
			return false;
	}

	/*  BFS -------------------------------------------------------------------
	 */
	void bfs(function<bool(node*, quad_t)> f) {
		if (root == nullptr)
			return;
		else if (root-> get_color() == node::BLACK) {
			f(root, origin);
			return;
		}

		queue<tuple<node*, quad_t>> Q;
		Q.push({root, origin});
		while(!Q.empty()) {
			auto [n, q] = Q.front();
			Q.pop();

			for (auto& t: quad_lookup) {
				auto c = n->get_child(t);
				auto qs = subdivide_zone(t, q);

				if (f(c, qs)) {
					Q.push({c, qs});
				}
			}
		}
	}

	/*  Region Search ---------------------------------------------------------
	 */
	void region_search(quad_t z, function<void(node*, quad_t)> f) {
		auto func =
			[z, &f] (node* n, quad_t q) {
				if ((n != nullptr) && intersects(q, z)) {

					if ((n->get_color() == node::BLACK)) {

						f(n, q);

						return false;
					}
					return true;
				}
				return false;
			};

		bfs(func);
	}

	/*  Region Search ---------------------------------------------------------
	 */
	void region_search_grey(quad_t z, function<void(node*, quad_t)> f) {
		auto func =
			[z, &f] (node* n, quad_t q) {
				if ((n != nullptr) && intersects(q, z)) {

					if ((n->get_color() == node::BLACK)) {
						return false;
					}

					f(n, q);
					return true;
				}
				return false;
			};

		bfs(func);
	}

public:

	/*  Constructor -----------------------------------------------------------
	 */
	quad_tree(double _lat, double _lon, list<data_t> _data = {})
	: data(_data)
	, origin({ {.0, .0}, {_lat, _lon} })
	{
		// cout << std::setprecision(std::numeric_limits<double>::digits10);
		// unsigned long long total_data = 0;
		// unsigned long long total_inserted = 0;
		for (auto& record: data) {
			if (insert(&record)) {
				// total_inserted += record.second;
			}
			// total_data += record.second;
		}
		// cout << "poblacion total insertados: " << total_inserted << endl;
		// cout << "poblacion total datos: " << total_data << endl;
	}

	/*  Find ------------------------------------------------------------------
	 */
	data_t* find(point_t p) {
		node* entry = nullptr;
		auto func =
			[&entry, p] (node* n) {
				if ((n->get_color() == node::BLACK) ) {

					if  (p == n->get_point())
						entry = n;
				}
			};

		search(p, func);
		if (entry != nullptr)
			return entry->get_data();
		else
			return nullptr;
	}

	/*  Descend ---------------------------------------------------------------
	 */
	node* descend(point_t p) {
		node* m = nullptr;
		auto func = [&m] (node* n) { m = n; };
		search(p, func);
		return m;
	}

	/*  Depth -----------------------------------------------------------------
	 */
	int depth(point_t p) {
		int d = -1;
		auto func = [&d] (node* n) { d++; };
		search(p, func);
		return d;
	}

	/*  Track -----------------------------------------------------------------
	 */
	vector<quad_enum> track(point_t p) {
		vector<quad_enum> path;

		auto func =
			[&path] (node* n, quad_enum t) {
				path.push_back(t);
			};

		search(p, func);
		return path;
	}

	/*  Get Total Population --------------------------------------------------
	 */
	unsigned long get_total_population(quad_t z) {
		unsigned long total_population = 0;

		auto func =
			[&total_population, z] (node* n, quad_t q) {
				if (contains(n->get_point(), z))
					total_population += n->get_data()->second;
			};

		region_search(z, func);
		return total_population;
	}

	/*  Get Total Cities ------------------------------------------------------
	 */
	unsigned long get_total_cities(quad_t z) {
		unsigned long total_cities = 0;

		auto func =
			[&total_cities, z] (node* n, quad_t q) {
				if (contains(n->get_point(), z))
					total_cities++;
			};

		region_search(z, func);
		return total_cities;
	}

	/*  Get Max Depth ---------------------------------------------------------
	 */
	size_t get_max_depth(quad_t z = {}) {
		if (z == quad_t({}))
			z = origin;

		double max_depth = 0.;

		auto func =
			[&max_depth, z] (node* n, quad_t q) {

				double depth = ceil(log2(get<0>(z.second)/get<0>(q.second)))+1;

				max_depth = (max_depth > depth) ? max_depth : depth;
			};

		region_search_grey(z, func);
		return max_depth;
	}

	/*  Get Depth Histogram ---------------------------------------------------
	 */
	map<size_t, size_t> get_depth_histogram(quad_t z = {}) {
		if (z == quad_t({}))
			z = origin;

		map<size_t, size_t> histogram;

		auto func =
			[&histogram, z] (node* n, quad_t q) {
				size_t depth = ceil(log2(get<0>(z.second)/get<0>(q.second)))+1;

				if (histogram.find(depth) == end(histogram))
					histogram[depth] = 1;
				else
					histogram[depth] += 1;
			};

		region_search(z, func);
		return histogram;
	}

	/*  Get Histogram 2D ------------------------------------------------------
	 */
	// vector<tuple<int, int, unsigned long>> get_histogram2d(int r) {
	// 	auto [p, d] = origin;
	// 	auto [dx, dy] = d;
	// 	auto [dxr, dyr] = dist_t{dx/r, dy/r};

	// 	for (int i = 0; i < r; ++i) {
	// 		for (int j = 0; j < r; ++j) {
	// 			quad_t z = {{2*dxr*i-dx+dxr, 2*dyr*j-dy+dyr}, {dxr, dyr}};

	// 			cout << i << "," << j << "," << get_total_cities(z) << endl;
	// 		}
	// 	}
	// }

	/*  Get Histogram 2D ------------------------------------------------------
	 */
	vector<tuple<int, int, unsigned long>> get_histogram2d(int rx, int ry) {
		auto [p, d] = origin;
		auto [dx, dy] = d;
		auto [dxr, dyr] = dist_t{dx/rx, dy/ry};

		vector<tuple<int, int, unsigned long>> hist;

		for (int i = 0; i < rx; ++i) {
			for (int j = 0; j < ry; ++j) {
				quad_t z = {{2*dxr*i-dx+dxr, 2*dyr*j-dy+dyr}, {dxr, dyr}};
				// cout << z << endl;
				hist.push_back({i, j, get_max_depth(z)});
			}
		}

		return hist;
	}

	/*  In Order Walk --------------------------------------------------------
	 */
	void inorder_walk() {
		inorder_walk(root);
		cout << endl;
	}

	static void inorder_walk(node* n) {
		cout << "(";
		for (auto t: {NE, NW, SE, SW})
			if (n->get_child(t) != nullptr)
				inorder_walk(n->get_child(t));
		cout << ")";
	}

	/*  Insert ----------------------------------------------------------------
	 */
	bool insert(data_t d) {
		node* n = nullptr;
		if ((n = insert(&d)) != nullptr) {
			data.push_back(d);

			auto& ref = data.back();
			n->set_data(&ref);

			return true;
		}
		return false;
	}

	/*  Insert ----------------------------------------------------------------
	 */
	node* insert(data_t* d) {
		point_t p = d->first;
		quad_t q = origin;
		quad_t qs = origin;
		// cout << "[insert]: " << p << endl;

		node* current = root;
		node* child   = current;

		if (root == nullptr) {
			root = new node(d, node::BLACK);

			// cout << "root: ";
			// debug(root, q, ORIGIN);
			// cout << "[inserting]: " << p << endl;
			// cout << "[done]" << endl << endl;

			size_++;
			return root;
		}

		// si ya existe un punto en el cuadrante
		if (current->get_color() == node::BLACK) {

			// si ya existe una ciudad en el mismo punto se detiene el insert
			if (p == current->get_point())
				return current;
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
			// debug(current, q, ORIGIN);

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
					return nullptr;

				auto [black, qb, tb] =
					subdivide(child, qs, child->get_point());

				black = child->make_child(tb);
			}
		}

		// cout << "[done]" << endl << endl;

		size_++;
		return child;
	}

/////////////////////////////////////////////////////////////////////////////////////////

	bool remove(point_t p){

		node* temp = descend(p);
		node* father = temp->get_father();
		data_t* data;
		int index;

		// se comprueba la existencia de la ciudad
		if(temp == nullptr)
			return false;

		// si solo existia el nodo root se borran los datos del nodo y se retorna
		if(temp == root) {
			delete root;
			root = nullptr;
			size_ -= 1;
			return true;
		}

		// es necesaria una referencia directa al padre para evitar casos problematicos
		// en que el nodo elimine referencias asi mismo usando temp->father->{first,second,...}
		father = temp->get_father();

		// se borran los datos del nodo y se reinicia
		for(int i=0; i<4; i++){
			if(temp == father->child[i])
				index = i;
		}
		delete father->child[index];
		father->child[index] = nullptr;
		size_ -= 1;

		// se COMPACTA el arbol de forma iterativa
		while(father != nullptr) {

			// se cuentan los colores de los nodos hijos
			int w = 0;
			int g = 0;
			int b = 0;

			if(father->child[0] == nullptr ) w++;
			else if(father->child[0]->get_color() == node::GREY ) g++;

			if(father->child[1] == nullptr ) w++;
			else if(father->child[1]->get_color() == node::GREY ) g++;

			if(father->child[2] == nullptr ) w++;
			else if(father->child[2]->get_color() == node::GREY ) g++;

			if(father->child[3] == nullptr ) w++;
			else if(father->child[3]->get_color() == node::GREY ) g++;

			b = 4 - g - w;

			// si solo quedan nodos blancos se eliminan y nodo padre se blanquea
			if(w == 4){

				temp = father->get_father();

				for(int i=0; i<4; i++){
					if(temp->child[i] == father)
						index = i;
				}
				delete temp->child[index];
				temp->child[index] = nullptr;
				father = temp; // para siguiente iteracion

			// si solo queda un nodo negro entonces reemplaza al nodo padre y se eliminan los hijos
			} else if (b == 1 && w == 3) {

				if(father->child[0] != nullptr && father->child[0]->get_color() == node::BLACK)
					index = 0;
				else if(father->child[1] != nullptr && father->child[1]->get_color() == node::BLACK)
					index = 1;
				else if(father->child[2] != nullptr && father->child[2]->get_color() == node::BLACK)
					index = 2;
				else if(father->child[3] != nullptr && father->child[3]->get_color() == node::BLACK)
					index = 3;

				// se hace blanco el nodo hijo
				data = father->child[index]->get_data();
				delete father->child[index];
				father->child[index] = nullptr;

				// se mueven los datos del hijo al padre y se cambia de gris a negro
				father->data = data;
				father->set_color(node::BLACK);
				father = father->get_father();// para siguiente iteracion

			// condicion de termino de compactacion (g >= 1 || b >= 2)
			} else {
				break;
			}
		}

		return true;
	}

/////////////////////////////////////////////////////////////////////////////////////////

	/*  Size ------------------------------------------------------------------
	 */
	size_t size() {
		return size_;
	}
};

} // namespace dsa

#endif
