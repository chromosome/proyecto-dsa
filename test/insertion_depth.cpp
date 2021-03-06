#include <bits/stdc++.h>
#include "../quad_tree.h"
#include "../utils.h"

using namespace std;
using namespace dsa;

void insertion_depth(string filename) {
	quad_tree tree(90.0, 180.0);

	auto data = read_data(filename);

	double avrg_depth = 0;
	int inserted = 0;
	int interval = 20000;
	for (const auto& record: data) {

		if (tree.insert(record)) {
			inserted++;

			avrg_depth += tree.depth(record.first);

			if (inserted%interval == 0) {
				cout << avrg_depth/interval << endl;
				avrg_depth = 0;
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	string filename;
	if (argc > 1)
		filename = argv[1];
	else {
		cout << "[Error] Debe ingresar un archivo de entrada" << endl;
		return -1;
	}

	insertion_depth(filename);

	return 0;
}
