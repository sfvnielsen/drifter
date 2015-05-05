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
#include <cstring>

#include "tree.h"
#include "sampler.h"
#include "iofilehandler.h"
using namespace std;

Sampler runNetwork(string, int, int, int, double, bool);

int main(int argc, char* argv[]) {
    string data_file_name;
    int num_iterations = 50;
    int num_burnin = 50;
    int thinning = 10;
    bool write_out = false;
    double holdoutFraction = 0;
    bool sampleHyperparameters = false;
    if (argc < 2) {
        string to_few_arguments =   "Not enough input arguments.\n"
                                    "Syntax:\n"
                                    "   drifter datafile [#Iterations = 100] [#Thinning = 10] [output-dir] [holdoutFraction] [sample hyperparameters?]\n"
                                    "\n"
                                    "Ex. drifter data/network.txt 10000 10 results/outfiles 0.05 0";
        cout << to_few_arguments << endl;
        return 0;
        //throw runtime_error(to_few_arguments);
    } else if (argc == 2) {
        data_file_name = (string) argv[1];
    } else if (argc == 3) {
        data_file_name = (string) argv[1];
        num_iterations = atoi(argv[2]) / 2;
        num_burnin = num_iterations;

    } else if (argc == 4) {
        data_file_name = (string) argv[1];
        num_iterations = atoi(argv[2]) / 2;
        num_burnin = num_iterations;
        thinning = atoi(argv[3]);
    } else if (argc == 5) {
        data_file_name = (string) argv[1];
        num_iterations = atoi(argv[2]) / 2;
        num_burnin = num_iterations;
        thinning = atoi(argv[3]);
        write_out = true;    
    } else if (argc == 6) {
        data_file_name = (string) argv[1];
        num_iterations = atoi(argv[2]) / 2;
        num_burnin = num_iterations;
        thinning = atoi(argv[3]);
        write_out = true;
        holdoutFraction = atof(argv[5]);
    } else if (argc == 7) {
        data_file_name = (string) argv[1];
        num_iterations = atoi(argv[2]) / 2;
        num_burnin = num_iterations;
        thinning = atoi(argv[3]);
        write_out = true;
        holdoutFraction = atof(argv[5]);
        sampleHyperparameters = strcmp(argv[6],"1")==0;
    } else {
        throw runtime_error("Too many input arguments");
        return 1;
    }
    // running on network
    if (sampleHyperparameters)
        cout << "Sampling hyperparameters." << endl;

    if (write_out){
        string out_dir = (string) argv[4];
        cout << "Writing results to: " << out_dir << endl;
    }

    Sampler sampling_result = runNetwork(data_file_name, num_iterations, num_burnin, thinning, holdoutFraction, sampleHyperparameters);
    if (write_out) {
        string out_dir = (string) argv[4];
        cout << "Writing results to " << out_dir << "...";
        sampling_result.writeResults(out_dir);
        cout << "Writing hypers to " << out_dir << "...";
        sampling_result.writeHypers(out_dir);
    }
    cout << "done!" << endl;
    return 0;
}
/**
 * Test a network specifed by an edge list when performing num_of_iterations
 */
Sampler runNetwork(string data_file_name, int num_of_iterations, int num_burnin, int thinning, double holdoutFraction, bool sampleHyperparameters) {
    cout << "Running on: " << data_file_name << endl;
    IoFileHandler data_file(data_file_name);
    Sampler sampler = Sampler(data_file.getDataEl(), 0.5, 0.5, 1.0, 1.0, holdoutFraction, sampleHyperparameters);
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    sampler.run(num_of_iterations, num_burnin, thinning);
    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << " sec.\n"
              << "mean elapsed time per regraft: " << elapsed_seconds.count() / ((double) num_of_iterations) << " sec." << endl
              << "Iterations per second: " << ((double) num_of_iterations+num_burnin) / (double)elapsed_seconds.count() << " ips" << endl
              << "Prediction accuracy of the last tree was: " << sampler.getLastTree().predAccuracy()*100 << " % correct" << endl;
    return sampler;
}

