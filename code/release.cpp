/**
    Project:        Parmugit - Release file
    Class:          Node
    Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
    Affiliation:    Technical University of Denmark
    Date:           January 2015
*/

#include <iostream>
#include <chrono> // chrono::timepoint, chono::system_clock
#include <stdexcept> // throwing runtime errors

#include "tree.h"
#include "sampler.h"
#include "iofilehandler.h"
using namespace std;

Sampler runNetwork(string,int,int,int);

int main(int argc, char* argv[]) {

    string data_file_name;
    int num_iterations;
    int num_burnin;
    int thinning;

    bool write_out;
    if (argc < 2) {
	throw runtime_error("Not enough input arguments");
    }else if (argc == 2) {
        data_file_name = (string) argv[1];
        num_iterations = 100/2;
        num_burnin = num_iterations;
        thinning = 10;
        write_out = false;
    } else if (argc == 3){
        data_file_name = (string) argv[1];
        num_iterations = atoi(argv[2])/2;
        num_burnin = num_iterations;
        thinning = 10;
        write_out = false;
    } else if (argc == 4){
        data_file_name = (string) argv[1];
        num_iterations = atoi(argv[2])/2;
        num_burnin = num_iterations;
        thinning = atoi(argv[3]);
        write_out = false;
    } else if (argc == 5){
        data_file_name = (string) argv[1];
        num_iterations = atoi(argv[2])/2;
        num_burnin = num_iterations;
        thinning = atoi(argv[3]);
        write_out = true;
    } else {
        throw runtime_error("Too many input arguments");
        return 1;
    }

    // running on network
    Sampler sampling_result = runNetwork(data_file_name,num_iterations,num_burnin,thinning);

    if (write_out) {
        string out_dir = (string) argv[4];
        cout << "Writing results to " << out_dir << "...";
        sampling_result.writeResults(out_dir);
    }

    cout << "done!" << endl;
	return 0;
}
/**
 * Test a network specifed by an edge list when performing num_of_iterations
 */
Sampler runNetwork(string data_file_name, int num_of_iterations, int num_burnin, int thinning){
    cout << "Running on: " << data_file_name << endl;
    IoFileHandler data_file(data_file_name);

    Sampler sampler = Sampler(data_file.getDataEl(),0.5, 0.5, 1, 1);

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    sampler.run(num_of_iterations, num_burnin, thinning);

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);


    std::cout << "finished computation at " << std::ctime(&end_time)
    << "elapsed time: " << elapsed_seconds.count() << " sec.\n"
    << "mean elapsed time per regraft: " << elapsed_seconds.count()/((double) num_of_iterations) << " sec." << endl
    << "Iterations per second: " << ((double) num_of_iterations)/(double)elapsed_seconds.count() << " ips" << endl;

    return sampler;
}

