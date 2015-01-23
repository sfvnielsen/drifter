/**
    Project:        Parmugit
    Class:          Sampler
    Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
    Affiliation:    Technical University of Denmark
    Date:           January 2014
*/

#include "sampler.h"
#include <iostream>
//#include <numeric>
#include <dirent.h>
#include <fstream>
#include <cstdio>
#include <cassert>

using namespace std;
/**
 * Initialize with a tree already constructed.
 * The Adjacency matrix must be kept in memory outside the function.
 */
Sampler::Sampler(Tree T, double alpha, double beta, int rho_plus, int rho_minus):
                          alpha(alpha), beta(beta), rho_plus(rho_plus), rho_minus(rho_minus){

    // Setting the initial values in the chain.
    chain.push_back(T);
    lastLogLik = T.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);
    likelihoods.push_back(lastLogLik);
}

/**
* Initialize with the naive tree building in the adjacency matrix.
*/
Sampler::Sampler(list<pair<int,int>> data_graph, double alpha, double beta, int rho_plus, int rho_minus):
                                                  alpha(alpha), beta(beta), rho_plus(rho_plus), rho_minus(rho_minus){
    // Constructing the adjacency list
    adjacencyList = Adj_list(data_graph);

    // Initialize the flat tree
    Tree T = Tree(&adjacencyList);

    // Initialize the binary tree
    //Tree T = Tree(&adjacencyList,"Binary");

    // Setting the initial values in the chain.
    chain.push_back(T);
    lastLogLik = T.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);
    likelihoods.push_back(lastLogLik);
}



/**
* Running the Metropolis Hastings sampler
* @param L: number of iterations
*/
void Sampler::run(int L){
    lastLogLik = likelihoods.back();

    // Initialize the random generator
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    // The number of iterations corresponding to 1% of the run.
    int step = max((int) L/100,10);

    for (int i=0; i<L; i++){

        // Create a proposal
        Tree proposal = chain.back();
        double move_ratio = proposal.regraft(alpha, beta, rho_plus, rho_minus);

        // Get Likelihoods times priors
        double propLogLik = proposal.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);

        // Calculate the acceptance ratio
        double a = exp(propLogLik-lastLogLik)*move_ratio;

        // Update the chain based on accept or reject.
        if(a>dis(gen)){
            chain.push_back(proposal);
            likelihoods.push_back(propLogLik);
            lastLogLik = propLogLik;
        }else{
            chain.push_back(chain.back());
            likelihoods.push_back(lastLogLik);
        }

        // Print status information
        if (((i+1) % step)==0){
            cout << "[Iteration: "<< i+1 << " of " << L << "] Accptance ration: " << a
            << " Loglikelihood: "<< lastLogLik << endl << endl << flush;
        }

    }
}

/**
* Running the Metropolis hastings sampler with thinning
* @param L: number of iterations
* @param thinning: save only each thinning'th sample
*/
void Sampler::run(int L, int thinning ){
    lastLogLik = likelihoods.back();
    Tree lastTree = chain.back();

    // Initialize the random generator
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    // The number of iterations corresponding to 1% of the run.
    int step = max((int) L/100,10);

    for (int i=0; i<L; i++){

        // Create a proposal
        Tree proposal = lastTree;
        double move_ratio = proposal.regraft(alpha, beta, rho_plus, rho_minus); //Try a move

        // Get Likelihoods times priors
        double propLogLik = proposal.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);

        // calculate the acceptance ratio
        double a = exp(propLogLik-lastLogLik)*move_ratio;
        // Following asserts do not work for some compilers
        //assert(!isnan(a));
        //assert(!isinf(propLogLik) );
        //assert(!isinf(lastLogLik) );

        // Update the chain based on accept or reject.
        if(a>dis(gen)){
            if ( (i%thinning) == 0) {
                chain.push_back(proposal);
                likelihoods.push_back(propLogLik);
            }
            lastLogLik = propLogLik;
            lastTree = proposal;
        }else{
            if ( (i%thinning) == 0) {
                chain.push_back(lastTree);
                likelihoods.push_back(lastLogLik);
            }
        }

        // Print status information
        if (((i) % step)==0){
            cout << "[Iteration: "<< (int)(i*100)/L << "% of " << L << "] Acceptance ratio: " << a
            << " Log-likelihood: "<< lastLogLik << endl << endl <<flush ;
        }
    }
}


/**
* Running the Metropolis hastings sampler with burnin and thinning
* @param L: number of iterations
* NB! Calls above run method after burn-in
*/
void Sampler::run(int L, int burn_in, int thinning){
    Tree oldTree = chain.back();
    Tree proposal = oldTree;

    // Initialize the random generator
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    // The number of iterations corresponding to 1% of the run.
    int bstep = max((int) burn_in/100,10);

    for (int i=0; i<burn_in; i++){

        // Create a proposal
        double move_ratio = proposal.regraft(alpha, beta, rho_plus, rho_minus);

        // Get Likelihoods times priors
        double propLogLik = proposal.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);

        // calculate the acceptance ratio
        double a = exp(propLogLik-lastLogLik)*move_ratio;
        // Following asserts do not work for some compilers
        //assert(!isnan(a));
        //assert(!isinf(propLogLik) );
        //assert(!isinf(lastLogLik) );

        // Update the chain based on accept or reject.
        if(a>dis(gen)){
            oldTree = proposal;
            lastLogLik = propLogLik;
        }else{
            proposal = oldTree;
        }

        // Print status information
        if (((i) % bstep)==0){
            cout << "[Burnin: "<< (int)(i*100)/burn_in << "% of " << burn_in << "] Acceptance ratio: " << a
            << " Log-likelihood: "<< lastLogLik << endl << endl <<flush ;
        }
    }

    // Set the chain to start at the end of burn-in.
    likelihoods.clear();
    likelihoods.push_back(lastLogLik);
    chain.clear();
    chain.push_back(oldTree);

    run(L,thinning);
}

/**
* Get the last Log(Likelihood times prior) (non-normalized posterior)
*/
double Sampler::getLastLogLikelihood(){
    return likelihoods.back();
}

/**
* Get the last tree in the Chain.
*/
Tree Sampler::getLastTree(){
    return chain.back();
}

/**
* Write results of sampling procedure.
*/
void Sampler::writeResults(std::string folder) {

    // If folder doesnt exist - create it yourself!
    DIR *dir;
    if ((dir = opendir (folder.c_str())) == NULL) {
        throw runtime_error("Target directory for writing results not found");
    }

    // Write trees
    int n_sample = 1;
    string filename = "";
    for (auto it = chain.begin(); it != chain.end(); ++it) {
            char buffer [6];
            sprintf (buffer,"%05d", n_sample);
            string n_id = (string) buffer;
            filename = folder + "/tree_sample" + n_id + ".txt";
            it->writeMatlabFormat(filename);
            n_sample++;
    }

    // Write likelihood
    filename = folder + "/loglikelihood.txt";
    ofstream out_file(filename);

    for (auto it = likelihoods.begin(); it != likelihoods.end(); ++it){
        out_file << *it << " ";
    }
}

/**
* Write the LogLikelihoods times priors (non-normalized posteriors) to a file.
*/
void Sampler::writeLogLikelihood(string folder){
    // If folder doesnt exist - create it
    DIR *dir;
    if ((dir = opendir (folder.c_str())) == NULL) {
        throw runtime_error("Target directory for writing results not found");
    }

    // Write likelihood
    string filename = folder + "/loglikelihood.txt";
    ofstream out_file(filename);

    for (auto it = likelihoods.begin(); it != likelihoods.end(); ++it){
        out_file << *it << " ";
    }
}
