/**
 *  Project:        Parmugit
 *  Class:          Sampler
 *  Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
 *  Affiliation:    Technical University of Denmark
 *  Date:           January 2015
 */

#include "sampler.h"
#include <iostream>
//#include <numeric>
#include <dirent.h>
#include <fstream>
#include <cstdio>
#include <cassert>
#include <stdexcept>


using namespace std;
/**
 * Initialize with a tree already constructed.
 * The Adjacency matrix must be kept in memory outside the function.
 */
Sampler::Sampler(Tree T, double alpha, double beta, double rho_plus, double rho_minus):
                          alpha(alpha), beta(beta), rho_plus(rho_plus), rho_minus(rho_minus){

    // Setting the initial values in the chain.
    chain.push_back(T);
    lastLogLik = T.evaluateLogLikeTimesPrior();
    likelihoods.push_back(lastLogLik);
    sample_hypers = true;
}

/**
* Initialize with the naive tree building in the adjacency matrix.
*/
Sampler::Sampler(list<pair<int,int>> data_graph, double alpha, double beta, double rho_plus, double rho_minus): alpha(alpha), beta(beta), rho_plus(rho_plus), rho_minus(rho_minus){
    // Constructing the adjacency list
    adjacencyList = Adj_list(data_graph);

    // Initialize the flat tree
    Tree T = Tree(&adjacencyList,alpha, beta, rho_plus, rho_minus);

    // Initialize the binary tree
    //Tree T = Tree(&adjacencyList,"Binary");

    // Setting the initial values in the chain.
    chain.push_back(T);
    lastLogLik = T.evaluateLogLikeTimesPrior();
    likelihoods.push_back(lastLogLik);
    sample_hypers = true;
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

        // Take back of chain and sample hyperparameters each 1% of the run
        Tree proposal = chain.back();
        if (((i+1) % step)==0 && sample_hypers) {
            proposal = sampleHyperparameters();
        }
        // Regraft and return move ratio
        double move_ratio = proposal.regraft();

        // Get Likelihoods times priors
        double propLogLik = proposal.evaluateLogLikeTimesPrior();

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
        // Take back of chain and sample hyperparameters each 1% of the run
        Tree proposal = chain.back();
        if ((i % step)==0 && sample_hypers) {
            proposal = sampleHyperparameters();
        }
        double move_ratio = proposal.regraft(); //Try a move

        // Get Likelihoods times priors
        double propLogLik = proposal.evaluateLogLikeTimesPrior();

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
        // Sample hyperparameters each 1% of the run

        if ((i % bstep)==0) {
            Tree proposal = sampleHyperparameters();
        }

        // Propose new tree
        double move_ratio = proposal.regraft();

        // Get Likelihoods times priors
        double propLogLik = proposal.evaluateLogLikeTimesPrior();

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
* Sample hyperparameters
*/

Tree Sampler::sampleHyperparameters() {
    // Number of sampling steps for each hyperparameter
    int n_resamples = 10;

    // Initialize the random generator
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> u_dis(0, 1);
    normal_distribution<> n_dis(0, 1);

    // Get latest tree
    Tree currentTree = chain.back();
    double previousLogLik = lastLogLik;

    // Sample alpha
    double old_alpha = currentTree.alpha;
    for (int n = 0; n != n_resamples; ++n) {
        // Propose new parameter by random walk
        double new_alpha = exp(log(currentTree.alpha)+0.1*n_dis(gen));
        currentTree.alpha = new_alpha;

        // Recalculate log-likelihood
        currentTree.initializeLogPrior();
        double propLogLik = currentTree.evaluateLogLikeTimesPrior();

        // calculate the acceptance ratio
        double a = exp(propLogLik-previousLogLik);

        // Update parameter based acceptance ratio
        if(a>u_dis(gen)){
            previousLogLik = propLogLik;
            old_alpha = new_alpha;
        }else{
            currentTree.alpha = old_alpha;
        }
    }
    // if last sample is rejected recalculate caches
    currentTree.initializeLogPrior();

    // Sample beta
    double old_beta = currentTree.beta;
    for (int n = 0; n != n_resamples; ++n) {
        // Propose new parameter by random walk
        double new_beta = exp(log(currentTree.beta)+0.1*n_dis(gen));
        currentTree.beta = new_beta;

        // Recalculate log-likelihood
        currentTree.initializeLogPrior();
        double propLogLik = currentTree.evaluateLogLikeTimesPrior();

        // calculate the acceptance ratio
        double a = exp(propLogLik-previousLogLik);

        // Update parameter based acceptance ratio
        if(a>u_dis(gen)){
            previousLogLik = propLogLik;
            old_beta = new_beta;
        }else{
            currentTree.beta = old_beta;

        }
    }
    // if last sample is rejected recalculate caches
    currentTree.initializeLogPrior();

    // Sample rho_plus
    double old_rho_plus = currentTree.rho_plus;
    for (int n = 0; n != n_resamples; ++n) {
        // Propose new parameter by random walk
        double new_rho_plus = exp(log(currentTree.rho_plus)+0.1*n_dis(gen));
        currentTree.rho_plus = new_rho_plus;

        // Recalculate log-likelihood
        currentTree.initializeLogLike();
        double propLogLik = currentTree.evaluateLogLikeTimesPrior();

        // calculate the acceptance ratio
        double a = exp(propLogLik-previousLogLik);

        // Update parameter based acceptance ratio
        if(a>u_dis(gen)){
            previousLogLik = propLogLik;
            old_rho_plus = new_rho_plus;
        }else{
            currentTree.rho_plus = old_rho_plus;

        }
    }
    // if last sample is rejected recalculate caches
    currentTree.initializeLogLike();

    // Sample rho_minus
    double old_rho_minus = currentTree.rho_minus;
    for (int n = 0; n != n_resamples; ++n) {
        // Propose new parameter by random walk
        double new_rho_minus = exp(log(currentTree.rho_minus)+0.1*n_dis(gen));
        currentTree.rho_minus = new_rho_minus;

        // Recalculate log-likelihood
        currentTree.initializeLogLike();
        double propLogLik = currentTree.evaluateLogLikeTimesPrior();

        // calculate the acceptance ratio
        double a = exp(propLogLik-previousLogLik);

        // Update parameter based acceptance ratio
        if(a>u_dis(gen)){
            previousLogLik = propLogLik;
            old_rho_minus = new_rho_minus;
        }else{
            currentTree.rho_minus = old_rho_minus;
        }
    }
    // if last sample is rejected recalculate caches
    currentTree.initializeLogLike();
    return currentTree;
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
* Get the "Best" tree in the Chain.
*/
Tree Sampler::getMapTree(){
    double lBest = 0.0;
    Tree tBest = chain.front();

    auto lit = likelihoods.begin();

    for(auto it = chain.begin(); it != chain.end(); it++){
        if(*lit >= lBest){
            lBest = *lit;
            tBest = *it;
        }
        lit++;
    }

    return tBest;
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

    // Write Holdout scores to file.
    filename = folder + "/scoresEnsemble.txt";
    ofstream scores(filename);

    for(auto it = chain.begin(); it != chain.end(); it++){
        scores << it->holdoutScores();
    }
    scores.close();

    // Write Holdout scores to file.
    filename = folder + "/scoresMAP.txt";
    scores.open(filename);
    scores << getMapTree().holdoutScores();
    scores.close();
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


/**
* Write hyper parameters to file
*/
void Sampler::writeHypers(string folder){
    // If folder doesnt exist - create it
    DIR *dir;
    if ((dir = opendir (folder.c_str())) == NULL) {
        throw runtime_error("Target directory for writing results not found");
    }

    // Write alpha
    string filename = folder + "/hyper_alpha.txt";
    ofstream out_file(filename);

    for (auto it = chain.begin(); it != chain.end(); ++it){
        out_file << it->alpha << " ";
    }

    // Write beta
    filename = folder + "/hyper_beta.txt";
    ofstream out_file2(filename);

    for (auto it = chain.begin(); it != chain.end(); ++it){
        out_file2 << it->beta << " ";
    }

    // Write rho_plus
    filename = folder + "/hyper_rhop.txt";
    ofstream out_file3(filename);

    for (auto it = chain.begin(); it != chain.end(); ++it){
        out_file3 << it->rho_plus << " ";
    }

    // Write rho_minus
    filename = folder + "/hyper_rhom.txt";
    ofstream out_file4(filename);

    for (auto it = chain.begin(); it != chain.end(); ++it){
        out_file4 << it->rho_minus << " ";
    }
}


