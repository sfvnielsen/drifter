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
        Sampler(Tree,double,double,double,double);
        Sampler(std::list<std::pair<int,int>>,double,double,double,double);

        // Various Run methods.
        void run(int);
        void run(int,int);
        void run(int,int,int);
        Tree sampleHyperparameters();

        // Accessing the end of the chain.
        Tree getLastTree();
        double getLastLogLikelihood();

        // Writing the results to files.
        void writeResults(std::string);
        void writeLogLikelihood(std::string);
        void writeHypers(std::string);

        // Data storage, publicity need for test function.
        std::list<Tree> chain;  //TODO vector
        std::list<double> likelihoods;  //TODO vector

    private:
        // Data storage
        Adj_list adjacencyList;
        double lastLogLik;

        // Hyperparameters:
        double alpha;
        double beta;
        double rho_plus;
        double rho_minus;
        bool sample_hypers;
};

#endif // SAMPLER_H
