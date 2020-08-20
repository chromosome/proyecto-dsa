#include <bits/stdc++.h>
#include "utils.h"
#include "quad_tree.h"

using namespace std;

int main(int argc, char const *argv[])
{
	string filename;

    filename = "test2.csv";
	dsa::quad_tree qtree(read_data(filename));

	cout << "inserted: " << qtree.size() << endl << endl;

    cout << endl;

	// profundidad de puntos ------------------------------------------------
	cout << "depth of " << "<-10.0, -10.0>" << ": " << qtree.depth({-10.0, -10.0}) << endl;
	cout << "depth of " << "<10.0, 10.0>" << ": " << qtree.depth({10.0, 10.0}) << endl;
	cout << "depth of " << "<-80.0, -80.0>" << ": " << qtree.depth({-80.0, -80.0}) << endl;
	cout << "depth of " << "<-80.2, -80.2>" << ": " << qtree.depth({-80.2, -80.2}) << endl;

	cout << endl;

	// remove -----------------------------------------------------------------
	if (qtree.remove({-80.0, -80.0}))
		cout << "removed:" << "<-80.0, -80.0>" << endl << endl;

	if(qtree.find({-80.0, -80.0}) == nullptr)
        cout << "fue eliminado" << endl << endl;

	// profundidad de puntos ------------------------------------------------
	cout << "depth of " << "<-10.0, -10.0>" << ": " << qtree.depth({-10.0, -10.0}) << endl;
	cout << "depth of " << "<10.0, 10.0>" << ": " << qtree.depth({10.0, 10.0}) << endl;
	cout << "depth of " << "<-80.2, -80.2>" << ": " << qtree.depth({-80.2, -80.2}) << endl;
	cout << endl;

	// remove -----------------------------------------------------------------
	if (qtree.remove({-10.0, -10.0}))
		cout << "removed:" << "-10.0, -10.0" << endl << endl;

	if(qtree.find({-10.0, -10.0}) == nullptr)
        cout << "fue eliminado" << endl << endl;

	// profundidad de puntos ------------------------------------------------
	cout << "depth of " << "<10.0, 10.0>" << ": " << qtree.depth({10.0, 10.0}) << endl;
	cout << "depth of " << "<-80.2, -80.2>" << ": " << qtree.depth({-80.2, -80.2}) << endl;
	cout << endl;

	// remove -----------------------------------------------------------------
	if (qtree.remove({10.0, 10.0}))
		cout << "removed:" << "10.0, 10.0" << endl << endl;

    // profundidad de puntos ------------------------------------------------
	cout << "depth of " << "<-80.2, -80.2>" << ": " << qtree.depth({-80.2, -80.2}) << endl;
	cout << endl;

/*
*/
	return 0;
}
