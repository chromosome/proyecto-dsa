#include <bits/stdc++.h>
#include "utils.h"
#include "quad_tree.h"

using namespace std;

int main(int argc, char const *argv[])
{
	// dsa::quad_tree qtree(read_data2("worldcitiespop_fixed.csv"));
	dsa::quad_tree qtree(read_data("test2.csv"));
	cout << "inserted: " << qtree.size() << endl;

	cout << endl;
	point_t city = {47,59};
	
	// data_t* d = qtree.search(p);
	// if (d)
	// 	cout << "search: " << d->first << ": " << d->second << endl;
	// else
	// 	cout << "not found!" << endl;
	int total_population = 0;
	quad_t quad = {{40, 40}, {40, 40}};
	qtree.bfs(quad, 
		[&total_population, quad](dsa::node* n) { 
				cout << n->get_data()->second << endl;
				total_population += n->get_data()->second;
		}
	);

	cout << "total population: " << total_population << endl;

	return 0;
}
