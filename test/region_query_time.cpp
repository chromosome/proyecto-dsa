#include <bits/stdc++.h>
#include "../quad_tree.h"
#include "../utils.h"

using namespace std;
using namespace dsa;

void region_query_time(string filename) {
	quad_tree tree(90.0, 180.0);

	auto data = read_data(filename);

	random_device r;
	default_random_engine re(r());

	for (const auto& record: data)
		tree.insert(record);

	int reps = 1000;
	for (int d = 1; d <= 25; d++) {

		auto f = 
			[&tree, &re, &d, reps] {
				for (int i = 0; i < reps; ++i) {

					uniform_real_distribution<> udx(-90. + d,  90. - d);
					uniform_real_distribution<> udy(-180. + d, 180. - d);

					double dx = udx(re);
					double dy = udy(re);

					dsa::quad_t q = { { dx, dy }, { d, d } };

					tree.get_total_population(q);
				}
			};

		cout << profile(f) << endl;
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

	region_query_time(filename);

	return 0;
}
