#include <bits/stdc++.h>
#include "../utils.h"
#include "../quad_tree.h"

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

	int s = 2;
	auto hist = qtree.get_population_histogram2d(s*90., s*180.);
	// auto hist = qtree.get_histogram2d(s, s);

	for (const auto& t: hist)
		cout << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << endl;

	return 0;
}
