//============================================================================
// Name        : release.cpp
// Author      : Julian, Jesper and Søren
// Version     : alpha
// Copyright   : It is OURS!!
// Description : Release mode version of alpha-program
//============================================================================

#include <iostream>
#include <cstdlib> // setting random seed
#include <time.h> // ----
#include <chrono> // chrono::timepoint, chono::system_clock

#include "barebones_tree.h"
#include "sampler.h"
#include "iofilehandler.h"
using namespace std;

Sampler runNetwork(string,int);

int main(int argc, char* argv[]) {
    srand ((unsigned int) time(NULL)); // set random seed

    string data_file_name;
    int num_iterations;
    bool write_out;
    if (argc < 2) {
        data_file_name = "data/karate_edgelist.txt";
        num_iterations = 10000;
        write_out = false;
    }else if (argc == 2) {
        data_file_name = (string) argv[1];
        num_iterations = 100;
        write_out = false;
    } else if (argc == 3){
        data_file_name = (string) argv[1];
        num_iterations = atoi(argv[2]);
        write_out = false;
    } else if (argc == 4){
        data_file_name = (string) argv[1];
        num_iterations = atoi(argv[2]);
        write_out = true;
    } else {
        throw runtime_error("Too many input arguments");
        return 1;
    }

    // running on network
    Sampler sampling_result = runNetwork(data_file_name,num_iterations);

    if (write_out) {
        string out_dir = (string) argv[3];
        cout << "Writing results to " << out_dir << "...";
        sampling_result.writeResults(out_dir);
    }

    cout << "done!" << endl;
	return 0;
}
/**
 * Test a network specifed by an edge list when performing num_of_iterations
 */
Sampler runNetwork(string data_file_name, int num_of_iterations){
    cout << "Running on: " << data_file_name << endl;
    IoFileHandler data_file(data_file_name,0);

    Tree new_tree(data_file.getDataEl()); // initialize flat tree

    Sampler sampler = Sampler(new_tree,0.5, 0.5, 1, 1);

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    sampler.run(num_of_iterations);

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);


    std::cout << "finished computation at " << std::ctime(&end_time)
    << "elapsed time: " << elapsed_seconds.count() << " sec.\n"
    << "mean elapsed time per regraft: " << elapsed_seconds.count()/((double) num_of_iterations) << " sec." << endl
    << "Iterations per second: " << ((double) num_of_iterations)/(double)elapsed_seconds.count() << " ips" << endl;

    return sampler;
}

