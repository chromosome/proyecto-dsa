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
