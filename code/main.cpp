//============================================================================
// Name        : main.cpp
// Author      : Julian
// Version     :
// Copyright   : It is OURS!!
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib> // setting random seed
#include <time.h> // ----

#include "barebones_tree.h"
#include "sampler.h"
using namespace std;

int main() {
    srand ((unsigned int) time(NULL)); // set random seed


	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

    // Building a tree with a graph defined by an edge list
    //Treee n4_03
	pair<int,int> g1 (0,1);
    pair<int,int> g2 (1,2);
    pair<int,int> g3 (0,3);
    pair<int,int> g4 (1,3);
    pair<int,int> g5 (2,3);
    list<pair<int,int>> data_edge_list = {g1,g2,g3,g4,g5};

    // tree - (binary )
    pair<int,int> t1 (0,1);
    pair<int,int> t2 (0,2);
    pair<int,int> t3 (1,3);
    pair<int,int> t4 (1,4);
    pair<int,int> t5 (2,5);
    pair<int,int> t6 (2,6);
    list<pair<int,int>> tree_edge_list = {t1,t2,t3,t4,t5,t6};

    // data-leaf relation - convention
    vector<int> data_leaf_relation(2*4,-1);
    data_leaf_relation[3] = 0;
    data_leaf_relation[4] = 3;
    data_leaf_relation[5] = 1;
    data_leaf_relation[6] = 2;


    cout << "--- Constructing the tree ---" << endl;
    Tree T = Tree(data_edge_list,tree_edge_list,data_leaf_relation);

    cout << "--- Tree to string ---" << endl;
    cout << T.toString() << endl;

    cout << "--- Likelihood ---" << endl;
    cout << "L:" << T.evaluateLogLikeTimesPrior(0.5, 0.5, 1, 1) << endl;

    cout << "--- New Tree ---" << endl;

    Tree T2 = T.regraft();

    cout << T2.toString() << endl;

    cout << "--- Likelihood ---" << endl;

    cout << "L:" << T2.evaluateLogLikeTimesPrior(0.5, 0.5, 1, 1) << endl;



//    cout << "--- Using the sampler object---" << endl;
//    Sampler S = Sampler(T,0.5, 0.5, 1, 1);

//    cout << "L:" << S.getLastLikelihood() << endl;
//
//    cout << "--------Get Random Node -------" << endl;
//    Node * random_node = T.getRandomNode();
//    cout << random_node->toString() << endl;

	return 0;
}
