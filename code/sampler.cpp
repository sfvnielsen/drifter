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
}
/**
* Initialize with the naive tree building in the adjacency matrix.
*/
Sampler::Sampler(list<pair<int,int>> data_graph, double alpha, double beta, int rho_plus, int rho_minus):
                                                  alpha(alpha), beta(beta), rho_plus(rho_plus), rho_minus(rho_minus){

    list<int> leaves;
        // insert all the indexes from the edge list into leaves
    for (list<pair<int,int>>::iterator it = data_graph.begin(); it != data_graph.end(); it++){
        leaves.push_back(it->first);
        leaves.push_back(it->second);
    }

    // Find only the unique elements
    leaves.sort();
    leaves.unique();

    int N = (int) leaves.size();
    adjacencyList = Adj_list(N,data_graph);

    Tree T = Tree(N,&adjacencyList);
    chain.push_back(T);
}
/**
*Initialize with tree based on data, tree and data-tree relation
*/
//Sampler::Sampler(list<pair<int,int>> data_graph, list<pair<int,int>> tree_struct_graph,
//                 list<pair<int,int>> data_leaf_relation, double alpha, double beta, int rho_plus, int rho_minus):
//                                                          alpha(alpha), beta(beta), rho_plus(rho_plus), rho_minus(rho_minus){
//    Tree T = Tree(data_graph,tree_struct_graph,data_leaf_relation);
//    chain.push_back(T);
//}


Sampler::~Sampler()
{
    //dtor
}
/**
* Running the Metropolis hastings sampler
* @param L: number of iterations
*/
void Sampler::run(int L){
double lastLogLik = -std::numeric_limits<double>::infinity();
for (int i=0; i<L; i++){
    // Create a proposal
    Tree proposal = chain.back();
//    cout << proposal.toString() << endl;
    double move_ratio = proposal.regraft(); //Try a move

    // Get Likelihoods times priors
    double propLogLik = proposal.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);

    // calculate the acceptance ratio
    double a = exp(propLogLik-lastLogLik)*move_ratio;
    if(a>=1){
        chain.push_back(proposal);
        likelihoods.push_back(propLogLik);
        lastLogLik = propLogLik;
    }else if(a>(double)rand()/RAND_MAX){
        chain.push_back(proposal);
        likelihoods.push_back(propLogLik);
        lastLogLik = propLogLik;
    }else{
        chain.push_back(chain.back());
        likelihoods.push_back(lastLogLik);
    }

    if (((i+1) % 10)==0){
        cout << "[Iteration: "<< i+1 << " of " << L << "] Accptance ration: " << a
        << " Loglikelihood: "<< lastLogLik << endl << endl << flush;

    }

}
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
