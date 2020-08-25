#include <bits/stdc++.h>
#include "../quad_tree.h"
#include "../utils.h"

using namespace std;
using namespace dsa;

void insertion_time(string filename) {
	quad_tree tree(90.0, 180.0);

	vector<data_t> inserted_data;

	for (const auto& record: read_data(filename)) {
		if (tree.insert(record))
			inserted_data.push_back(record);
	}

	random_device r;
	default_random_engine re(r());
	uniform_real_distribution<> ud(0, inserted_data.size());

	shuffle(begin(inserted_data), end(inserted_data), re);

	double elapsed = 0.;
	int queried = 0;
	int interval = 20000;
	for (const auto& record: inserted_data) {

		queried++;
		elapsed += profile([&tree, record] { tree.find(record.first); });

		if (queried%interval == 0) {
			cout << elapsed/interval << endl;
			elapsed = 0.;
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
