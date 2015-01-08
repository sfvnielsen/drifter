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

    cout << "--- TESTING the node class ---" <<endl;


    // Doing is manually
    // No nodes list

//    Node A = Node(2);
//    cout << "A: " << A.toString() << endl;
//    Node B = Node(19);
//    cout << "B: " << B.toString() << endl;
//
//    Node R = Node();
//    cout << "R: " << R.toString() << endl;
//    R.addChild(&A);
//    R.addChild(&B);
//    cout << "R: " << R.toString() << endl;

    // WORKS

//// With a nodes list
//    list<Node> nodes;
//    Node R = Node();
//
//    nodes.push_back(Node(2));
//    cout << "A: " << nodes.back().toString() << endl;
//    R.addChild(&(nodes.back()));
//
//    cout << "R: " << R.toString() << endl;
//
//    nodes.push_back(Node(19));
//    cout << "B: " << nodes.back().toString() << endl;
//    R.addChild(&(nodes.back()));
//
//    cout << "R: " << R.toString() << endl;

    // WORKS

//// with a loop
//list<int> leaves;
//leaves.push_back(3);
//leaves.push_back(17);
//list<Node> nodes;
//Node R = Node();
//
//for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
//        nodes.push_back(Node(*it));
//        cout << *it << ": " <<nodes.back().toString() << endl;
//        R.addChild(&(nodes.back()));
//        cout << "R: " << R.toString() << endl;
//    }
//
//

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

    // Building a tree with a graph defined by an edge list
	tuple<int,int> g1 (0,1);
    tuple<int,int> g2 (1,2);
    tuple<int,int> g3 (1,3);
    tuple<int,int> g4 (3,2);

    cout << "--- Constructing the tree ---" << endl;
    Tree T = Tree({g1,g2,g3,g4});

    cout << "--- Tree to string ---" << endl;
    cout << T.toString() << endl;

	return 0;
}
