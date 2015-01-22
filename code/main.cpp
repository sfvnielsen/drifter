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
#include <chrono> // chrono::timepoint, chrono::system_clock

#include "tree.h"
#include "sampler.h"
#include "iofilehandler.h"
#include <random>

using namespace std;

void testNetwork(string,int,int,int);

int main() {
    srand ((unsigned int) time(NULL)); // set random seed

    std::random_device rd;
    std::mt19937 random_generator(rd());

    string data_file_name = "data/karate_edgelist.txt";
    //data_file_name = "data/celegans_edgelist.txt";
    //data_file_name = "data/football_edgelist.txt";
    //data_file_name = "data/facebook100_edgelist.txt";
    //data_file_name = "data/uspower_edgelist.txt";

    /**
     * Testing a network
     */
    int num_iterations = 100000;
    int burnin = 100;
    int thinning = 100;

    testNetwork(data_file_name,num_iterations,burnin,thinning);

	return 0;
}
/**
 * Test a network specifed by an edge list when performing num_of_iterations
 */
void testNetwork(string data_file_name, int num_of_iterations, int burnin, int thinning){
    cout << "Running on: " << data_file_name << endl;
    IoFileHandler data_file(data_file_name,0);

    Sampler sampler = Sampler(data_file.getDataEl(), 0.5, 0.5, 1, 1);

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    sampler.run(num_of_iterations, burnin, thinning);

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    //cout << sampler.getLast().toString() << endl;

    std::cout << "finished computation at " << std::ctime(&end_time)
    << "elapsed time: " << elapsed_seconds.count() << " sec.\n"
    << "mean elapsed time per regraft: " << elapsed_seconds.count()/((double) num_of_iterations) << " sec." << endl << endl
    << "Iterations per second: " << ((double) num_of_iterations)/(double)elapsed_seconds.count() << " ips" << endl;
}
