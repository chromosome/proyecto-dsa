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

	qtree.get_histogram2d(90., 180.);

	return 0;
}
