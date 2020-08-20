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

	bool search(point_t p, function<void(node*, point_t)> f = nullptr) {
		if (f == nullptr)
			f = [](node* n, point_t p){};

		node* current = root;
		quad_t q = origin;

		while ((current != nullptr) && (current->get_color() == node::GREY)) {
			f(current, p);

            quad_enum t;
            tie(current, q, t) = subdivide(current, q, p);
        }

		if (current != nullptr) {
			f(current, p);
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

	quad_tree(vector<data_t> _data, double _lat = 90.0, double _lon = 180.0)
	: data(_data)
	, origin({ {.0, .0}, {_lat, _lon} })
	{
		// cout << std::setprecision(std::numeric_limits<double>::digits10);
		for (auto& record: data)
			insert(&record);
	}

	data_t* find(point_t p) {
		node* entry = nullptr;
		auto func = 
			[&entry] (node* n, point_t p) {
				if ((n->get_color() == node::BLACK) && (p == n->get_point()))
					entry = n;
			};
		search(p, func);
		if (entry != nullptr)
			return entry->get_data();
		else
			return nullptr;
	}

	node* descend(point_t p) {
		node* m = nullptr;
		auto func = 
			[&m] (node* n, point_t p) {
				m = n;
			};
		search(p, func);
		return m;
	}

	int depth(point_t p) {
		int d = 0;
		auto func = [&d] (node* n, point_t p) { d++; };
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

	map<size_t, size_t> get_depth_histogram() {
		map<size_t, size_t> histogram;
		auto func = 
			[&histogram, this] (node* n, quad_t q) { 
				if (n != nullptr) {
					if (n->get_color() == node::BLACK) {
						size_t depth = 
							log2(get<0>(origin.second)/get<0>(q.second));
						if (histogram.find(depth) == end(histogram))
							histogram[depth] = 1;
						else
							histogram[depth] += 1;
						return false;
					}
					return true;
				}
				return false;
			};
		bfs(func);
		return histogram;
	}

	bool insert(data_t* d) {
		point_t p = d->first;
		quad_t q = origin;
		quad_t qs = origin;
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

		// si ya existe un punto en el cuadrante
		if (current->get_color() == node::BLACK) {

            // si ya existe una ciudad en el mismo punto se detiene el insert
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

/////////////////////////////////////////////////////////////////////////////////////////

	node* search_city(point_t p) {
		node* current = root;
		quad_t q = quad;

		while ((current != nullptr) && current->get_color() == node::GREY) {
			auto t = quadrant::quadrant_of(p, q.first);
			q = quadrant::subdivide_zone(t, q);
			current = current->get_child(t);
		}

		if (current != nullptr && p == current->get_point())
			return current;

		return nullptr;
	}

    bool remove(point_t p){

        node* temp = search_city(p);
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


	size_t size() {
		return size_;
	}
};

} // namespace dsa

#endif
