//============================================================================
// Name        : main.cpp
// Author      : Julian
// Version     :
// Copyright   : It is OURS!!
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "barebones_tree.h"
using namespace std;

int main() {

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

    // Building a tree with a graph defined by an edge list
	pair<int,int> g1 (0,1);
    pair<int,int> g2 (1,2);
    pair<int,int> g3 (0,3);
    pair<int,int> g4 (1,3);
    pair<int,int> g5 (2,3);

    cout << "--- Constructing the tree ---" << endl;
    Tree T = Tree({g1,g2,g3,g4,g5});

    cout << "--- Tree to string ---" << endl;
    cout << T.toString() << endl;

    cout << "--- Likelihood ---" << endl;

    cout << "L:" << T.evaluateLogLikeTimesPrior(0.5, 0.5, 1, 1) << endl;

    cout << "--- Remove rightmost subtree of root ---" << endl;

    Node * r = T.getRandomNode();
    T.cutSubtree(r);
    cout << "L:" << T.evaluateLogLikeTimesPrior(0.5, 0.5, 1, 1) << endl;

    cout << "--- Insert as child of root---" << endl;
    T.insertSubtree(T.getRoot(), r, true);

    cout << "L:" << T.evaluateLogLikeTimesPrior(0.5, 0.5, 1, 1) << endl;
    r = T.getRandomNode();
    T.cutSubtree(r);


    cout << T.toString();

    cout << "--- Insert as sibling of rightmost subtree of root---" << endl;
    Node * t = T.getRandomNode();
    T.insertSubtree(t, r, false);

    cout << "L:" << T.evaluateLogLikeTimesPrior(0.5, 0.5, 1, 1) << endl;

    cout << T.toString();


    r = T.getRandomNode();
    T.cutSubtree(r);

    cout << "--- Insert as child of rightmost subtree of root---" << endl;

    T.insertSubtree(t, r, true);
    cout << "L:" << T.evaluateLogLikeTimesPrior(0.5, 0.5, 1, 1) << endl;



	return 0;
}
