#include <bits/stdc++.h>
#include "../quad_tree.h"
#include "../utils.h"

using namespace std;
using namespace dsa;

void insertion_time(string filename) {
	quad_tree tree(90.0, 180.0);

	ifstream input(filename);
	string line;

	getline(input, line);
	while (getline(input, line)) {
		stringstream line_stream(line);
		string field;

		vector<string> record;
		while (getline(line_stream, field, ';'))
			record.push_back(field);

		profile([&tree, record] { tree.insert(make_entry(record)); });
	}
}

void insertion_depth(string filename) {
	quad_tree tree(90.0, 180.0);

	auto data = read_data(filename);

	double d = 0.;
	for (const auto& record: data) {
		if (tree.insert(record)) {
			d += tree.depth(record.first);
			d /= tree.size();
			cout << d << endl;
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
