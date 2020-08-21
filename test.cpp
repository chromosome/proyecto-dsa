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

	// qtree.inorder_walk();

	// // dsa::quad_t q = {{110., 0.}, {30, 30}};
	// dsa::quad_t q = {{2000., 0.}, {30, 30}};

	profile([&qtree] { qtree.inorder_walk(); });
}
