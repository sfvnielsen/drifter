/**
 *  Project:        Parmugit
 *  Class:          Sampler
 *  Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
 *  Affiliation:    Technical University of Denmark
 *  Date:           January 2015
 */

#ifndef SAMPLER_H
#define SAMPLER_H

#include "tree.h"
#include <random>
#include <cstdlib>

class Sampler
{
    public:
        // Constructors
        Sampler();
        Sampler(Tree,double,double,int,int);
        Sampler(std::list<std::pair<int,int>>,double,double,int,int);

        // Various Run methods.
        void run(int);
        void run(int,int);
        void run(int,int,int);

        // Accessing the end of the chain.
        Tree getLastTree();
        double getLastLogLikelihood();

        // Writing the results to files.
        void writeResults(std::string);
        void writeLogLikelihood(std::string);
    
        // Data storage, publicity need for test function.
        std::list<Tree> chain;
        std::list<double> likelihoods;

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
