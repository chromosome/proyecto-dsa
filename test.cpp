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
	auto data = read_data(filename);
	dsa::quad_tree tree(90., 180.);

	for (const auto& record: data)
		tree.insert(record);

	// auto tree = make_quad_tree(filename, 90., 180.);
	cout << "inserted: " << tree.size() << endl << endl;

	// busqueda ---------------------------------------------------------------
	point_t city = {50.523429, 29.461285};
	for (const auto& record: data) {
		data_t d = tree.find(record.first);
		if (d != data_t())
			cout << "found: " << d.first << ": " << d.second << endl << endl;
		else
			cout << "not found!" << endl << endl;
	}

	// Borrar -----------------------------------------------------------------
	if (tree.remove(city))
		cout << "removed" << endl << endl;
	else
		cout << "not removed" << endl << endl;

	data_t d = tree.find(city);
	if (d != data_t())
		cout << "found: " << d.first << ": " << d.second << endl << endl;
	else
		cout << "not found!" << endl << endl;

	tree.insert({{50.523429, 29.461285}, 10});

	d = tree.find(city);
	if (d != data_t())
		cout << "found: " << d.first << ": " << d.second << endl << endl;
	else
		cout << "not found!" << endl << endl;


	// Puntos por region ------------------------------------------------------

	dsa::quad_t q = {{0, 0}, {200, 200}};
	cout << "total cities: " << tree.get_total_cities(q) << endl << endl;

	profile([&tree] { tree.inorder_walk(); });
}
