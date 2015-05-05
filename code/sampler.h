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
#include <utility> //Pair
#include <list>

class Sampler
{
    public:
        // Constructors
        Sampler();
        Sampler(Tree,double,double,double,double);
        Sampler(std::list<std::pair<int,int>>,double,double,double,double);
        Sampler(std::list<std::pair<int,int>>,double,double,double,double,double);
        Sampler(std::list<std::pair<int,int>>,double,double,double,double,double,bool);
        Sampler(std::list<std::pair<int,int>>,double,double,double,double,double,bool,bool,bool,bool);


        // Various Run methods.
        void run(int);
        void run(int,int);
        void run(int,int,int);
        void run(int,int,int,int);
        Tree sampleHyperparameters();

        // Accessing the end of the chain.
        Tree getLastTree();
        Tree getMapTree();
        double getLastLogLikelihood();
        Tree getMAPTree();
        std::vector<std::pair<int, double>>buildCredibilityTree(Tree);


        std::string toString(std::list<std::pair<std::pair<int,int>,std::pair<double,bool>>>);
        std::list<std::pair<std::pair<int,int>,std::pair<double,bool>>> meanScores(std::list<std::pair<std::pair<int,int>,std::pair<double,bool>>>);

        // Writing the results to files.
        void writeResults(std::string);
        void writeLogLikelihood(std::string);
        void writeHypers(std::string);

        // Data storage, publicity need for test function.
        std::list<Tree> chain;  //TODO vector
        std::list<double> likelihoods;  //TODO vector

    private:
        std::list<std::pair<Node *, std::pair<int,int> > > calcSubtreeCred(Node *,std::list<Node*>,int);


        // Data storage
        Adj_list adjacencyList;
        double lastLogLik;

        // Hyperparameters:
        double alpha;
        double beta;
        double rho_plus;
        double rho_minus;
        bool sample_alpha;
        bool sample_beta;
        bool sample_rho_plus;
        bool sample_rho_minus;
};

#endif // SAMPLER_H
