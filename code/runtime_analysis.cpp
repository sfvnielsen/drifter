/**
 *  Project:        Parmugit - Runtime analysis
 *  Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
 *  Affiliation:    Technical University of Denmark
 *  Date:           January 2015
 */

#include <iostream>
#include <time.h> // ----
#include <chrono> // chrono::timepoint, chrono::system_clock

#include "sampler.h"
#include "iofilehandler.h"
#include <random>

using namespace std;

pair<double,double> testNetwork(string,int,int,int);

int main() {
    srand ((unsigned int) time(NULL)); // set random seed

    std::random_device rd;
    std::mt19937 random_generator(rd());


    /** Fixed node size experiment */
    list<string> data_names = {"data/synthetic_data/fix_size/data_1.txt",
                                "data/synthetic_data/fix_size/data_2.txt",
                                "data/synthetic_data/fix_size/data_3.txt",
                                "data/synthetic_data/fix_size/data_4.txt",
                                "data/synthetic_data/fix_size/data_5.txt"
                                };

    list<double> l_runtime;
    list<double> l_ips;

    /**
     * Testing a network
     */
    int num_iterations = 10000;
    int burnin = 10000;
    int thinning = 100;
    for (auto it = data_names.begin(); it!=data_names.end(); ++it){
            pair<double,double> result = testNetwork(*it,num_iterations,burnin,thinning);
            l_runtime.push_back(result.first);
            l_ips.push_back(result.second);
    }

    /**
        Printing statistics
    */
    auto it_r = l_runtime.begin();
    auto it_i = l_ips.begin();
    cout << endl << endl << "Runtime statistics:" <<endl;

    printf("%30s \t %10s \t %10s \n"," ","elapsed [s]","ips");
    for (auto it = data_names.begin(); it!=data_names.end(); ++it){
            printf("%30s \t %10f \t %10f \n",(*it).c_str(),*it_r++,*it_i++);
    }


	return 0;
}
/**
 * Test a network specifed by an edge list when performing num_of_iterations
 */
pair<double,double> testNetwork(string data_file_name, int num_of_iterations, int burnin, int thinning){
    pair<double,double> result;
    cout << "Running on: " << data_file_name << endl;
    IoFileHandler data_file(data_file_name,0);

    Sampler sampler = Sampler(data_file.getDataEl(), 0.5, 0.5, 1, 1);

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    sampler.run(num_of_iterations, burnin, thinning);

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    result.first = (double)elapsed_seconds.count();
    result.second = ((double) num_of_iterations)/(double)elapsed_seconds.count();
    std::cout << "finished computation at " << std::ctime(&end_time)
    << "elapsed time: " << result.first << " sec.\n"
    << "mean elapsed time per regraft: " << elapsed_seconds.count()/((double) num_of_iterations) << " sec." << endl << endl
    << "Iterations per second: " << result.second << " ips" << endl;
    return result;
}
