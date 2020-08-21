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
	dsa::quad_tree qtree(4., 4., read_data(filename));


	// // insercion --------------------------------------------------------------
	// dsa::quad_tree qtree(4., 4.);

	// qtree.insert({{1., 3.}, 5000});
	// qtree.insert({{3.5, 3.5}, 5000});
	// qtree.insert({{2.25, 2.25}, 5000});
	// qtree.insert({{2.25, 2.75}, 5000});

	cout << "inserted: " << qtree.size() << endl << endl;

	// busqueda ---------------------------------------------------------------
	// point_t city = {50,29};
	point_t city = {1., 3.};
	data_t* d = qtree.find(city);
	if (d)
		cout << "found: " << d->first << ": " << d->second << endl << endl;
	else
		cout << "not found!" << endl << endl;
	
	// tracking
	auto path = qtree.track(city);

	cout << "path: ";
	for (const auto& p: path)
		cout << quad_map[p+1] << " ";
	cout << endl << endl;


	// profundidad de un punto ------------------------------------------------
	cout << "depth of " << city << ": " << qtree.depth(city) << endl << endl;
	// point_t city = {50,29};
	city = {3.5, 3.5};
	d = qtree.find(city);
	if (d)
		cout << "found: " << d->first << ": " << d->second << endl << endl;
	else
		cout << "not found!" << endl << endl;
	
	// tracking
	path = qtree.track(city);

	cout << "path: ";
	for (const auto& p: path)
		cout << quad_map[p+1] << " ";
	cout << endl << endl;


	// profundidad de un punto ------------------------------------------------
	cout << "depth of " << city << ": " << qtree.depth(city) << endl << endl;
	// point_t city = {50,29};
	city = {2.25, 2.25};
	d = qtree.find(city);
	if (d)
		cout << "found: " << d->first << ": " << d->second << endl << endl;
	else
		cout << "not found!" << endl << endl;
	
	// tracking
	path = qtree.track(city);

	cout << "path: ";
	for (const auto& p: path)
		cout << quad_map[p+1] << " ";
	cout << endl << endl;


	// profundidad de un punto ------------------------------------------------
	cout << "depth of " << city << ": " << qtree.depth(city) << endl << endl;
	// point_t city = {50,29};
	city = {2.25, 2.75};
	d = qtree.find(city);
	if (d)
		cout << "found: " << d->first << ": " << d->second << endl << endl;
	else
		cout << "not found!" << endl << endl;
	
	// tracking
	path = qtree.track(city);

	cout << "path: ";
	for (const auto& p: path)
		cout << quad_map[p+1] << " ";
	cout << endl << endl;

	// // conteo -----------------------------------------------------------------
	// dsa::quad_t q = {{40, 40}, {5, 5}};
	// // dsa::quad_t q = {{0, 0}, {90, 180}};
	// cout << get_zone(q) << endl;

	// cout << "total cities: " << qtree.get_total_cities(q) << endl << endl;

	// cout << "total population: " << qtree.get_total_population(q) << endl << endl;

	// cout << "max depth: " << qtree.get_max_depth() << endl << endl;

	// // histograma -------------------------------------------------------------
	// auto h = qtree.get_depth_histogram();
	// for (auto [k, v]: h)
	// 	cout << k << ": " << v << endl;
	// cout << endl;

	// operaciones entre cuadrantes -------------------------------------------
	dsa::quad_t x = {{0, 0}, {2, 2}};
	dsa::quad_t z = {{3, 3}, {2, 2}};

	// interseccion
	cout << get_zone(x) << " y " << get_zone(z) << " ";
	if (intersects(x, z))
		cout << "intersectan" << endl;
	else
		cout << "no intersectan" << endl;

	// z contiene x
	x = {{0, 0}, {1, 1}};
	z = {{0, 0}, {2, 2}};
	// x = {{0, 0}, {2, 2}};
	// z = {{0, 0}, {1, 1}};

	if (contains(x, z))
		cout << get_zone(z) << " contiene a " << get_zone(x) << endl;
	else
		cout << get_zone(z) << " no contiene a " << get_zone(x) << endl;

}
