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
Tree debuggingTree();

int main() {
    srand ((unsigned int) time(NULL)); // set random seed

    string data_file_name = "data/karate_edgelist.txt";
    //data_file_name = "data/celegans_edgelist.txt";
//    data_file_name = "data/football_edgelist.txt";
    //data_file_name = "data/facebook100_edgelist.txt";


    cout << "--- Original tree --- " << endl;
    Tree T = debuggingTree();

    cout << T.toString() << endl;
    cout << "--- Copied tree --- " << endl;
    Tree T2 = Tree(T);
    cout << T2.toString() << endl;

    /**
     * Testing a network
     */
//    int num_iterations = 1000;
//
//    testNetwork(data_file_name,num_iterations);
//
	return 0;
}
/**
 * Test a network specifed by an edge list when performing num_of_iterations
 */
void testNetwork(string data_file_name, int num_of_iterations){
    cout << "Running on: " << data_file_name << endl;
    IoFileHandler data_file(data_file_name,0);
    data_file.read_graph();

    Tree new_tree(data_file.getDataEl(),"Binary"); // initialize flat tree
    /*******/
//    Tree new_tree = debuggingTree(); //DEBUG
    cout << new_tree.toString() << endl;

//    cout << new_tree.toString() << endl;

    Sampler sampler = Sampler(new_tree,0.5, 0.5, 1, 1);

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    sampler.run(num_of_iterations);

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    cout << sampler.getLast().toString() << endl;

    std::cout << "finished computation at " << std::ctime(&end_time)
    << "elapsed time: " << elapsed_seconds.count() << " sec.\n"
    << "mean elapsed time per regraft: " << elapsed_seconds.count()/((double) num_of_iterations) << " sec." << endl << endl
    << "Iterations per second: " << ((double) num_of_iterations)/(double)elapsed_seconds.count() << " ips" << endl;
}

Tree debuggingTree(){
    pair<int,int> g1 (0,1);
    pair<int,int> g2 (1,2);
    pair<int,int> g3 (0,3);
    pair<int,int> g4 (1,3);
    pair<int,int> g5 (2,3);
    list<pair<int,int>> data_edge_list = {g1,g2,g3,g4};

    return Tree(data_edge_list);
}
