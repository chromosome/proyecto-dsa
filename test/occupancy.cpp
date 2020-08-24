#include <bits/stdc++.h>
#include "../quad_tree.h"
#include "../utils.h"

using namespace std;
using namespace dsa;

void ocuppancy(string filename) {
	quad_tree tree(90.0, 180.0);

	auto data = read_data(filename);

	int interval = 10000;
	int i = 1;
	for (const auto& record: data) {
		if (tree.insert(record)) {
			if (i%interval == 0) {
				auto [w, g, b] = tree.node_count();
				cout << w << "," << g << "," << b << endl;
				++i;
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

	ocuppancy(filename);

	return 0;
}
