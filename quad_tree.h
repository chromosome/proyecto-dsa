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
		child->set_father(n);
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

		// si ya existe un punto en el cuadrante
		if (current->get_color() == node::BLACK) {

            // si ya existe una ciudad en el mismo punto se detiene el insert
			if (p == current->get_point())
				return false;
			else // subdivide para generar nodo hijo y volver al nodo current color gris
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
				child->set_father(current);

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

    bool remove(data_t* d){

		point_t p = d->first;

        node* temp = search_city(p);
        node* father;
        data_t* data;

        // se comprueba la existencia de la ciudad
        if(temp == nullptr)
            return false;

        // si solo existia el nodo root se borran los datos del nodo y se retorna
        if(temp == root) {
            delete temp;
            temp = nullptr;
            size_ -= 1;
            return true;
        }

        // es necesaria una referencia directa al padre para evitar casos problematicos
        // en que el nodo elimine referencias asi mismo usando temp->father->{first,second,...}
        father = temp->get_father();

        // se borran los datos del nodo y se reinicia
        delete temp;
        temp = nullptr;
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
                delete father;
                father = temp; // para siguiente iteracion

            // si solo queda un nodo negro entonces reemplaza al nodo padre y se eliminan los hijos
            } else if (b == 1 && w == 3) {

                int index;

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

                // se mueven los datos del hijo al padre
                father->data = data;
                father = father->get_father();// para siguiente iteracion

            // condicion de termino de compactacion (g >= 1 || b >= 2)
            } else {
                return false;
            }
        }
    }

/////////////////////////////////////////////////////////////////////////////////////////


	size_t size() {
		return size_;
	}
};

}

#endif
