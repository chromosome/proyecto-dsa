#include <bits/stdc++.h>
#include "utils.h"
#include "quad_tree.h"

using namespace std;

int main(int argc, char const *argv[])
{
	string filename;
	if (argc > 1)
		filename = argv[1];
	else {
		cout << "[Error] Debe ingresar un archivo de entrada" << endl;
		return -1;
	}

	dsa::quad_tree qtree(read_data(filename));
	cout << "inserted: " << qtree.size() << endl << endl;

	// busqueda ---------------------------------------------------------------
	point_t city = {50,29};
	// point_t city = {50.523429, 29.461285};
	data_t* d = qtree.find(city);
	if (d)
		cout << "search: " << d->first << ": " << d->second << endl << endl;
	else
		cout << "not found!" << endl << endl;

	// profundidad de un punto ------------------------------------------------
	cout << "depth of " << city << ": " << qtree.depth(city) << endl << endl;

	// conteo -----------------------------------------------------------------
	long int total_population = 0;
	quad_t q = {{40, 40}, {2, 2}};
	// quad_t q = {{0, 0}, {90, 180}};
	cout << dsa::quadrant::get_zone(q) << endl;
	qtree.region_search(q);

	cout << "total population: " << total_population << endl;

	// remove -----------------------------------------------------------------
	data_t* city2 = new data_t({{25, 39}, 1240810});
	if (qtree.insert(city2))
		cout << "inserted:" << city2->first << endl;

	if (qtree.remove(city2))
		cout << "removed:" << city2->first << endl;

	return 0;
}
