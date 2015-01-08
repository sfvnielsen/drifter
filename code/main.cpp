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

	tuple<int,int> g1 (0,1);
    tuple<int,int> g2 (1,2);
    tuple<int,int> g3 (1,3);
    tuple<int,int> g4 (3,2);

    Tree T = Tree({g1,g2,g3,g4});
    cout << T.toString() << endl;
	return 0;
}
