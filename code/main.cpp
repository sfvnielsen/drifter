/**
    Project:        Parmugit (main file)
    Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
    Affiliation:    Technical University of Denmark
    Date:           January 2014
*/

#include <iostream>
#include <time.h> // ----
#include <chrono> // chrono::timepoint, chrono::system_clock

#include "sampler.h"
#include "iofilehandler.h"
#include <random>

using namespace std;

void testNetwork(string,int,int,int);

int main() {

    string data_file_name = "data/karate_edgelist.txt";
    //data_file_name = "data/celegans_edgelist.txt";
    //data_file_name = "data/football_edgelist.txt";
    //data_file_name = "data/facebook100_edgelist.txt";
    //data_file_name = "data/uspower_edgelist.txt";

    /**
     * Testing a network
     */
    int num_iterations = 1000;
    int burnin = 1000;
    int thinning = 10;

    testNetwork(data_file_name,num_iterations,burnin,thinning);

	return 0;
}
/**
 * Test a network specifed by an edge list when performing num_of_iterations
 */
void testNetwork(string data_file_name, int num_of_iterations, int burnin, int thinning){
    // Loading the data.
    cout << "Running on: " << data_file_name << endl;
    IoFileHandler data_file(data_file_name,0);

    // Constructing the sampler.
    Sampler sampler = Sampler(data_file.getDataEl(), 0.5, 0.5, 1, 1);

    // Start timing
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    // Run the sampler
    sampler.run(num_of_iterations, burnin, thinning);

    // End timing
    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    // Print the last tree.
    //cout << sampler.getLast().toString() << endl;

    // Print out statistics for the run.
    std::cout << "finished computation at " << std::ctime(&end_time)
    << "elapsed time: " << elapsed_seconds.count() << " sec.\n"
    << "mean elapsed time per regraft: " << elapsed_seconds.count()/((double) num_of_iterations) << " sec." << endl << endl
    << "Iterations per second: " << ((double) num_of_iterations)/(double)elapsed_seconds.count() << " ips" << endl;
}
