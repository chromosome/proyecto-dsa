#include <bits/stdc++.h>
#include "../quad_tree.h"
#include "../utils.h"

using namespace std;
using namespace dsa;

void query_time(string filename) {
	quad_tree tree(90.0, 180.0);

	auto data = read_data(filename);

	random_device r;
	default_random_engine re(r());
	uniform_int_distribution<int> ud(0, data.size());

	for (const auto& record: data)
		tree.insert(record);

	int reps = 1000;
	double elapsed = 0.;

	int count = 1'000'000;
	// int count = 100'000;
	for (int i = 1000; i <= count; i+=1000) {
		// cout << i << ": ";
		
		auto p = data[ud(re)].first;

		cout << profile( 

			[&tree, &p, &i] {
				for (int j = 0; j < i; ++j) {
					tree.find(p);
				}
			}

		) << endl;
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

	query_time(filename);

	return 0;
}
