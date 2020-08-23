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

	int s = 5;
	auto hist = qtree.get_depth_histogram();

	for (const auto& [d, c]: hist)
		cout << d << "," << c << endl;

	return 0;
}
