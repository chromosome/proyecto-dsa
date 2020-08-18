#include <bits/stdc++.h>
#include "utils.h"
#include "3/quad_tree.h"

using namespace std;

int main(int argc, char const *argv[])
{
	// dsa::quad_tree qtree(read_data2("worldcitiespop_fixed.csv"));
	dsa::quad_tree qtree(dsa::read_data2("test2.csv"));
	cout << qtree.size() << endl;

	// data_t* d = qtree.find({50.0, 29.0});
	// cout << d->first << ": " << d->second << endl;

	return 0;
}
