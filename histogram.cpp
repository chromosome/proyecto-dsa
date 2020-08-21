#include <bits/stdc++.h>
#include "utils.h"
#include "quad_tree.h"

using namespace std;
using namespace dsa;
using namespace quadrant;

int main(int argc, char const *argv[])
{
	// insercion --------------------------------------------------------------
	dsa::quad_tree qtree(90., 180., read_data("worldcitiespop_fixed.csv"));

	int s = 5;
	auto hist = qtree.get_histogram2d(s*90., s*180.);
	// auto hist = qtree.get_histogram2d(3, 3);

	for (const auto& t: hist)
		cout << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << endl;

	return 0;
}
