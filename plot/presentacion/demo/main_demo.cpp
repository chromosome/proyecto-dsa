#include <bits/stdc++.h>
#include "utils.h"
#include "quad_tree.h"

using namespace std;
using namespace dsa;

int main(int argc, char const *argv[])
{

    // se crea un quad_tree para hacer pruebas
    quad_tree qtree(256.0,256.0);

    // puntos de prueba
    data_t p1, p2, p3, p4, p5, p6, p7, p8;

    //          x        y      dato
    p1 = { {  128.0  , -128.0} ,  0};
    p2 = { {  128.0  ,  128.0} ,  1};
    p3 = { {  -16.0  ,   16.0} ,  2};
    p4 = { {  -16.0  ,   48.0} ,  3};
    p5 = { { -240.0  ,  240.0} ,  4};

    // se inserta p1 y se comprueba el estado del quadtree
    cout << "///////////////////////////////////////////" << endl;
    qtree.insert(p1);
    cout << "total puntos en quadtree: " << qtree.size() << endl;
    cout << "depth p1: " << qtree.depth(p1.first) << endl;

    // se inserta p2 y se comprueba el estado del quadtree
    cout << "///////////////////////////////////////////" << endl;
    qtree.insert(p2);
    cout << "total puntos en quadtree: " << qtree.size() << endl;
    cout << "depth p1: " << qtree.depth(p1.first) << endl;
    cout << "depth p2: " << qtree.depth(p2.first) << endl;

    // se inserta p3 y se comprueba el estado del quadtree
    cout << "///////////////////////////////////////////" << endl;
    qtree.insert(p3);
    cout << "total puntos en quadtree: " << qtree.size() << endl;
    cout << "depth p1: " << qtree.depth(p1.first) << endl;
    cout << "depth p2: " << qtree.depth(p2.first) << endl;
    cout << "depth p3: " << qtree.depth(p3.first) << endl;

    // se inserta p4 y se comprueba el estado del quadtree
    cout << "///////////////////////////////////////////" << endl;
    qtree.insert(p4);
    cout << "total puntos en quadtree: " << qtree.size() << endl;
    cout << "depth p1: " << qtree.depth(p1.first) << endl;
    cout << "depth p2: " << qtree.depth(p2.first) << endl;
    cout << "depth p3: " << qtree.depth(p3.first) << endl;
    cout << "depth p4: " << qtree.depth(p4.first) << endl;

    // se inserta p5 y se comprueba el estado del quadtree
    cout << "///////////////////////////////////////////" << endl;
    qtree.insert(p5);
    cout << "total puntos en quadtree: " << qtree.size() << endl;
    cout << "depth p1: " << qtree.depth(p1.first) << endl;
    cout << "depth p2: " << qtree.depth(p2.first) << endl;
    cout << "depth p3: " << qtree.depth(p3.first) << endl;
    cout << "depth p4: " << qtree.depth(p4.first) << endl;
    cout << "depth p5: " << qtree.depth(p5.first) << endl;

    // se remueve p4 y se comprueba el estado del quadtree
    cout << "///////////////////////////////////////////" << endl;
    qtree.remove(p4.first);
    cout << "total puntos en quadtree: " << qtree.size() << endl;
    cout << "depth p1: " << qtree.depth(p1.first) << endl;
    cout << "depth p2: " << qtree.depth(p2.first) << endl;
    cout << "depth p3: " << qtree.depth(p3.first) << endl;
    //cout << "depth p4: " << qtree.depth(p4.first) << endl;
    cout << "depth p5: " << qtree.depth(p5.first) << endl;

    // se inserta p4 y se comprueba el estado del quadtree
    cout << "///////////////////////////////////////////" << endl;
    qtree.insert(p4);
    cout << "total puntos en quadtree: " << qtree.size() << endl;
    cout << "depth p1: " << qtree.depth(p1.first) << endl;
    cout << "depth p2: " << qtree.depth(p2.first) << endl;
    cout << "depth p3: " << qtree.depth(p3.first) << endl;
    cout << "depth p4: " << qtree.depth(p4.first) << endl;
    cout << "depth p5: " << qtree.depth(p5.first) << endl;



    //std::cout << "\x1B[2J\x1B[H";


	data_t* d = qtree.find(p1.first);
	if (d)
		cout << "found: " << d->first << ": " << d->second << endl << endl;
	else
		cout << "not found!" << endl << endl;


	d = qtree.find(p2.first);
	if (d)
		cout << "found: " << d->first << ": " << d->second << endl << endl;
	else
		cout << "not found!" << endl << endl;

	d = qtree.find(p3.first);
	if (d)
		cout << "found: " << d->first << ": " << d->second << endl << endl;
	else
		cout << "not found!" << endl << endl;

	d = qtree.find(p4.first);
	if (d)
		cout << "found: " << d->first << ": " << d->second << endl << endl;
	else
		cout << "not found!" << endl << endl;

	d = qtree.find(p5.first);
	if (d)
		cout << "found: " << d->first << ": " << d->second << endl << endl;
	else
		cout << "not found!" << endl << endl;


    /*
    string filename;

    filename = "test2.csv";
	quad_tree qtree(90.0,180.0,read_data(filename));

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


*/
	return 0;
}
