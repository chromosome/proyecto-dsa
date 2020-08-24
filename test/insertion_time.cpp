#include <bits/stdc++.h>
#include "../quad_tree.h"
#include "../utils.h"

using namespace std;
using namespace dsa;

void insertion_time(string filename) {
	quad_tree tree(90.0, 180.0);

	auto data = read_data(filename);

	double elapsed = 0.;
	int inserted = 0;
	int interval = 20000;
	for (const auto& record: data) {

		if (tree.insert(record)) {
			inserted++;

			tree.remove(record.first);
			elapsed += profile([&tree, record] { tree.insert(record); });

			if (inserted%interval == 0) {
				cout << elapsed/interval << endl;
				elapsed = 0.;
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

	insertion_time(filename);

	return 0;
}
