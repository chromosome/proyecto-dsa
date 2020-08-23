#include <bits/stdc++.h>
#include "utils.h"
#include "quad_tree.h"

using namespace std;
using namespace dsa;
using namespace quadrant;

int main(int argc, char const *argv[])
{
	string filename;
	if (argc > 1)
		filename = argv[1];
	else {
		cout << "[Error] Debe ingresar un archivo de entrada" << endl;
		return -1;
	}

	// insercion --------------------------------------------------------------
	dsa::quad_tree qtree(90., 180., read_data(filename));
	cout << "inserted: " << qtree.size() << endl << endl;

	// busqueda ---------------------------------------------------------------
	// point_t city = {50,29};
	point_t city = {50.523429, 29.461285};
	data_t d = qtree.find(city);
	if (d != data_t{ })
		cout << "found: " << d.first << ": " << d.second << endl << endl;
	else
		cout << "not found!" << endl << endl;

	// profundidad de un punto ------------------------------------------------
	cout << "depth of " << city << ": " << qtree.depth(city) << endl << endl;

	// conteo -----------------------------------------------------------------
	// dsa::quad_t q = {{40, 40}, {0.1, 0.1}};
	dsa::quad_t q = {{0, 0}, {200, 200}};
	cout << get_zone(q) << endl;

	cout << "total cities: " << qtree.get_total_cities(q) << endl << endl;

	cout << "total population: " << qtree.get_total_population(q) << endl << endl;

	cout << "max depth: " << qtree.get_max_depth() << endl << endl;

	// histograma -------------------------------------------------------------
	auto h = qtree.get_depth_histogram();
	for (auto [k, v]: h)
		cout << k << ": " << v << endl;
	cout << endl;

	qtree.get_depth_histogram2d(4, 4);

	// Conteo de nodos --------------------------------------------------------
	auto [w, g, b] = qtree.node_count();
	cout << "blancos: " << w << endl;
	cout << "grises: "  << g << endl;
	cout << "negros: "  << b << endl;

	return 0;
}
