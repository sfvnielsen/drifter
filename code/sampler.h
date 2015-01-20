#ifndef SAMPLER_H
#define SAMPLER_H

#include "barebones_tree.h"
#include <cstdlib>

class Sampler
{
    public:
        Sampler();
        Sampler(Tree T, double , double , int , int );
        // Initialize with the naive tree building in the adjacency matrix.
        Sampler(std::list<std::pair<int,int>> , double ,double , int , int );

        void run(int); // run int samples
        void run(int,int); // run with thinning
        void run(int, int, int); // run burn-in (and call above after)
        Tree getLast();
        double getLastLikelihood();

        void writeResults(std::string);
        void writeLogLikelihood(std::string);
    
        std::list<Tree> chain;
        std::list<double> likelihoods;
    protected:

    private:
        // Data storage
        Adj_list adjacencyList;
        double lastLogLik;

        // Hyperparameters:
        double alpha;
        double beta;
        int rho_plus;
        int rho_minus;

};

#endif // SAMPLER_H
