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
#include <chrono> // chrono::timepoint, chono::system_clock

#include "barebones_tree.h"
#include "sampler.h"
#include "iofilehandler.h"
using namespace std;

void testNetwork(string,int);

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

    string data_file_name = "data/karate_edgelist.txt";
//    data_file_name = "data/celegans_edgelist.txt";
    
    int num_iterations = 10;
    /**
     * Testing a network
     */
    testNetwork(data_file_name,num_iterations);

    cout << "--- Using the sampler object---" << endl;
    Sampler sampler = Sampler(T,0.5, 0.5, 1, 1);
    sampler.run(10);

//    cout << "L:" << S.getLastLikelihood() << endl;
//
//    cout << "--------Get Random Node -------" << endl;
//    Node * random_node = T.getRandomNode();
//    cout << random_node->toString() << endl;

    
    cout << "--- END ---" << endl;
    
    
	return 0;
}
/**
 * Test a network specifed by an edge list when performing num_of_iterations
 */
void testNetwork(string data_file_name, int num_of_iterations){
    IoFileHandler data_file(data_file_name,0);
    data_file.read_graph();
    
    Tree new_tree(data_file.getDataEl()); // initialize flat tree
    cout << new_tree.toString() << endl;
    
    
    //    Tree new_tree(T);
    
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    for (int i = 0; i < num_of_iterations; i++){
        //        cout << "--- After regrafting "+to_string(i)+ "--- " << endl;
        new_tree.regraft();
        double llike = new_tree.evaluateLogLikeTimesPrior(.5, .5, 1, 1);
        cout << "Loglikelihood: "+ to_string(llike) << endl << endl;
        
    }
    cout << new_tree.toString() << endl;
    
    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    
    std::cout << "finished computation at " << std::ctime(&end_time)
    << "elapsed time: " << elapsed_seconds.count() << " sec.\n"
    << "mean elapsed time per regraft: " << elapsed_seconds.count()/((double) num_of_iterations) << " sec." << endl;
}
