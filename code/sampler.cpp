#include "sampler.h"
#include <iostream>
#include <numeric>
#include <dirent.h>
#include <fstream>
#include <cstdio>

using namespace std;

Sampler::Sampler(Tree T, double alpha, double beta, int rho_plus, int rho_minus):
                          alpha(alpha), beta(beta), rho_plus(rho_plus), rho_minus(rho_minus){

    chain.push_back(T);
    lastLogLik = T.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);
    likelihoods.push_back(lastLogLik);

}
/**
* Initialize with the naive tree building in the adjacency matrix.
*/
Sampler::Sampler(list<pair<int,int>> data_graph, double alpha, double beta, int rho_plus, int rho_minus):
                                                  alpha(alpha), beta(beta), rho_plus(rho_plus), rho_minus(rho_minus){
    adjacencyList = Adj_list(data_graph);

    Tree T = Tree(&adjacencyList);
    chain.push_back(T);
    lastLogLik = T.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);
    likelihoods.push_back(lastLogLik);
}



/**
* Running the Metropolis hastings sampler
* @param L: number of iterations
*/
void Sampler::run(int L){

    lastLogLik = likelihoods.back();

    int step = max((int) L/100,10);

    for (int i=0; i<L; i++){
        // Create a proposal
        Tree proposal = chain.back();

        double move_ratio = proposal.regraft(); //Try a move

        // Get Likelihoods times priors
        double propLogLik = proposal.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);

        // calculate the acceptance ratio
        double a = exp(propLogLik-lastLogLik)*move_ratio;
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, 1);
        if(a>dis(gen)){
            chain.push_back(proposal);
            likelihoods.push_back(propLogLik);
            lastLogLik = propLogLik;
        }else{
            chain.push_back(chain.back());
            likelihoods.push_back(lastLogLik);
        }

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

    int step = max((int) L/100,10);

    for (int i=0; i<L; i++){
        // Create a proposal
        Tree proposal = lastTree;

        double move_ratio = proposal.regraft(); //Try a move

        // Get Likelihoods times priors
        double propLogLik = proposal.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);

        // calculate the acceptance ratio
        double a = exp(propLogLik-lastLogLik)*move_ratio;
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, 1);
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

        if (((i) % step)==0){
            cout << "[Iteration: "<< (int)(i*100)/L << "% of " << L << "] Acceptance ratio: " << a
            << " Log-likelihood: "<< lastLogLik << endl << endl <<flush ;
        }

    }
}


/**
* Running the Metropolis hastings sampler with burnin and thinning
* @param L: number of iterations
*/
void Sampler::run(int L, int burnin, int thinning){

    Tree oldTree = chain.back();
    // Create a proposal
    Tree proposal = oldTree;

    int bstep = max((int) burnin/100,10);

    for (int i=0; i<burnin; i++){

        double move_ratio = proposal.regraft(); //Try a move

        // Get Likelihoods times priors
        double propLogLik = proposal.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);

        // calculate the acceptance ratio
        double a = exp(propLogLik-lastLogLik)*move_ratio;
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, 1);
        if(a>dis(gen)){
            oldTree = proposal;
            lastLogLik = propLogLik;
        }else{
            proposal = oldTree;
        }

        if (((i) % bstep)==0){
            cout << "[burnin: "<< (int)(i*100)/burnin << "% of " << burnin << "] Acceptance ratio: " << a
            << " Log-likelihood: "<< lastLogLik << endl << endl <<flush ;
        }
    }
    likelihoods.clear();
    likelihoods.push_back(lastLogLik);
    chain.clear();
    chain.push_back(oldTree);

    run(L,thinning);
}




double Sampler::getLastLikelihood(){
    return likelihoods.back();
}

Tree Sampler::getLast(){
    return chain.back();
}

/**
* Write results of sampling procedure
*/

void Sampler::writeResults(std::string folder) {

    // if folder doesnt exist - create it
    DIR *dir;
    if ((dir = opendir (folder.c_str())) == NULL) {
        throw runtime_error("Target directory for writing results not found");
    }

    // write trees
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

    // write likelihood
    filename = folder + "/loglikelihood.txt";
    ofstream out_file(filename);

    for (auto it = likelihoods.begin(); it != likelihoods.end(); ++it){
        out_file << *it << " ";
    }
}

void Sampler::writeLogLikelihood(string folder){
    // if folder doesnt exist - create it
    DIR *dir;
    if ((dir = opendir (folder.c_str())) == NULL) {
        throw runtime_error("Target directory for writing results not found");
    }

    // write likelihood
    string filename = folder + "/loglikelihood.txt";
    ofstream out_file(filename);

    for (auto it = likelihoods.begin(); it != likelihoods.end(); ++it){
        out_file << *it << " ";
    }
}


