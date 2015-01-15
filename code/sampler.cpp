#include "sampler.h"
#include <iostream>
#include <numeric>

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
    Tree T = Tree(data_graph);
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

    if (((i+1) % 50)==0){
        cout << "[Iteration: "<< i+1 << " of " << L << "] Accptance ration: " << a
        << " Loglikelihood: "<< lastLogLik << endl << endl << flush;

    }

}
}

double Sampler::getLastLikelihood(){
    return likelihoods.back();
}
