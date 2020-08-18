#include <bits/stdc++.h>
#include "utils.h"
#include "quad_tree2.h"

using namespace std;

int main(int argc, char const *argv[])
{
	// auto data = read_data("worldcitiespop_fixed.csv");
	// auto data = read_data2("test.csv");

	// dsa::quad_tree qtree(data);
	// cout << qtree.size << endl;

	// qtree.bfs();

	// cin.get();
	// qtree.print();

	// auto v = qtree.find({40.920404, 39.19209})->val();
	// cout << v << endl;

	dsa::quad_tree qtree(read_data2("worldcitiespop_fixed.csv"));
	// dsa::quad_tree qtree(read_data2("test.csv"));
	cout << qtree.size() << endl;

	return 0;
}
