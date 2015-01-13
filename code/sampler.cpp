#include "sampler.h"
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
*/
void Sampler::run(int L){
for (int i=0; i<L; i++){
    // Create a proposal
    Tree proposal = chain.back().regraft();

    // Get Likelihoods times priors
    float propLogLik = proposal.evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);
    float lastLogLik = chain.back().evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);

    // calculate the acceptance ratio
    float a = exp(propLogLik)/exp(lastLogLik);
    if(a>=1){
        chain.push_back(proposal);
    }else if(a>(double)rand()/RAND_MAX){
        chain.push_back(proposal);
    }else{
        chain.push_back(chain.back());
    }

}
}

float Sampler::getLastLikelihood(){
    return chain.back().evaluateLogLikeTimesPrior(alpha, beta, rho_plus, rho_minus);
}