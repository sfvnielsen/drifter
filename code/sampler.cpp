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
#include <limits>
#include <string>
#include <map>

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
    sample_alpha = true;
    sample_beta = true;
    sample_rho_plus = true;
    sample_rho_minus = true;

}

/**
* Initialize with the naive tree building in the adjacency matrix.
*/
Sampler::Sampler(list<pair<int,int>> data_graph, double alpha, double beta, double rho_plus, double rho_minus)
            : Sampler(data_graph, alpha, beta, rho_plus, rho_minus, 0.0, true, true,true, true) {}

/**
* Initialize with the naive tree building in the adjacency matrix.
*/
Sampler::Sampler(list<pair<int,int>> data_graph, double alpha, double beta, double rho_plus, double rho_minus, double holdoutFrac)
            : Sampler(data_graph, alpha, beta, rho_plus, rho_minus, holdoutFrac, true, true,true, true) {}

/**
* Initialize with the naive tree building in the adjacency matrix.
*/
Sampler::Sampler(list<pair<int,int>> data_graph, double alpha, double beta, double rho_plus, double rho_minus, double holdoutFrac,bool sampleHypers)
            : Sampler(data_graph, alpha, beta, rho_plus, rho_minus, holdoutFrac, sampleHypers, sampleHypers,sampleHypers, sampleHypers) {}



/**
* Initialize with the naive tree building in the adjacency matrix.
*/
Sampler::Sampler(list<pair<int,int>> data_graph, double alpha, double beta, double rho_plus, double rho_minus, double holdoutFraction, bool samp_alpha,
                 bool samp_beta, bool samp_rho_plus, bool samp_rho_minus)
            : alpha(alpha), beta(beta), rho_plus(rho_plus), rho_minus(rho_minus), sample_alpha(samp_alpha), sample_beta(samp_beta),
            sample_rho_plus(samp_rho_plus), sample_rho_minus(samp_rho_minus){
    // Constructing the adjacency list
    adjacencyList = Adj_list(data_graph,holdoutFraction);

    // Initialize the flat tree
    Tree T = Tree(&adjacencyList,alpha, beta, rho_plus, rho_minus);

    // Initialize the binary tree
    //Tree T = Tree(&adjacencyList,"Binary");

    // Setting the initial values in the chain.
    chain.push_back(T);
    lastLogLik = T.evaluateLogLikeTimesPrior();
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

        // Take back of chain and sample hyperparameters each 1% of the run
        Tree proposal = chain.back();
        if (((i+1) % step)==0 && (sample_alpha || sample_beta || sample_rho_plus || sample_rho_minus)) {
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
        if ((i % step)==0 && (sample_alpha || sample_beta || sample_rho_plus || sample_rho_minus)) {
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

    // Initialize the random generators
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    // The number of iterations corresponding to 1% of the run.
    int bstep = max((int) burn_in/100,10);

    for (int i=0; i<burn_in; i++){
        // Sample hyperparameters each 1% of the run

        if ((i % bstep)==0 && (sample_alpha || sample_beta || sample_rho_plus || sample_rho_minus)) {
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
* Running the Metropolis hastings sampler with burnin, thinning and
* a specified number of iterations between hyperparameter sampling
* @param L: number of iterations
* NB! Calls above run method after burn-in
*/
void Sampler::run(int L, int burn_in, int thinning, int hyper_thinning){
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

        if ((i % hyper_thinning)==0 && (sample_alpha || sample_beta || sample_rho_plus || sample_rho_minus)) {
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
    Tree lastTree = chain.back();

     // The number of iterations corresponding to 1% of the run.
    int step = max((int) L/100,10);

    for (int i=0; i<L; i++){
        // Take back of chain and sample hyperparameters each 1% of the run
        Tree proposal = chain.back();
        if ((i % hyper_thinning)==0 && (sample_alpha || sample_beta || sample_rho_plus || sample_rho_minus)) {
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
            << " Log-likelihood: "<< lastLogLik << endl << flush;
            cout << "Alpha: " << lastTree.alpha << " Beta: " << lastTree.beta << " Rho_Plus: "
            << lastTree.rho_plus << " Rho_minus: " << lastTree.rho_minus << endl << flush ;
        }
    }
}



/**
* Sample hyperparameters
*/

Tree Sampler::sampleHyperparameters() {
    // Number of sampling steps for each hyperparameter
    int n_resamples = 100;
    double stepsize = 1; // step size in random walker

    // Initialize the random generator
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> u_dis(0, 1);
    uniform_real_distribution<> n_dis(-1, 1);

    // Get latest tree
    Tree currentTree = chain.back();
    double previousLogLik = lastLogLik;
    double propLogLik = -INFINITY;

    // Sample alpha
    if (sample_alpha)
    {
        double old_alpha = currentTree.alpha;
        for (int n = 0; n != n_resamples; ++n)
        {
            // Propose new parameter by random walk
            double new_alpha = currentTree.alpha+stepsize*n_dis(gen);
            currentTree.alpha = new_alpha;
            if ( (new_alpha > 1) || (new_alpha < 0) ) {
                propLogLik = -INFINITY;
            } else {
                // Recalculate log-likelihood
                currentTree.initializeLogPrior();
                propLogLik = currentTree.evaluateLogLikeTimesPrior();
            }

            // calculate the acceptance ratio
            double a = exp(propLogLik-previousLogLik);

            // Update parameter based acceptance ratio
            if(a>u_dis(gen))
            {
                previousLogLik = propLogLik;
                old_alpha = new_alpha;
            }
            else
            {
                currentTree.alpha = old_alpha;
            }
        }
        // if last sample is rejected recalculate caches
        currentTree.initializeLogPrior();

    }

    // Sample beta
    if (sample_beta){
        double old_beta = currentTree.beta;
        for (int n = 0; n != n_resamples; ++n) {
            // Propose new parameter by random walk
            double new_beta = currentTree.beta+stepsize*n_dis(gen);
            currentTree.beta = new_beta;

            if ( (new_beta + currentTree.alpha < 0) ){ // beta + alpha > 0 must always hold
                propLogLik = -INFINITY;
            } else {
                // Recalculate log-likelihood
                currentTree.initializeLogPrior();
                propLogLik = currentTree.evaluateLogLikeTimesPrior();
            }

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
    }


    // Sample rho_plus
    if (sample_rho_plus) {
        double old_rho_plus = currentTree.rho_plus;
        for (int n = 0; n != n_resamples; ++n) {
            // Propose new parameter by random walk
            double new_rho_plus = currentTree.rho_plus+stepsize*n_dis(gen);
            currentTree.rho_plus = new_rho_plus;
            if (new_rho_plus < 0) {
                    propLogLik = -INFINITY;
            }else {
                // Recalculate log-likelihood
                    currentTree.initializeLogLike();
                    propLogLik = currentTree.evaluateLogLikeTimesPrior();
            }

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

    }


    // Sample rho_minus
    if (sample_rho_minus) {
        double old_rho_minus = currentTree.rho_minus;
        for (int n = 0; n != n_resamples; ++n) {
            // Propose new parameter by random walk
            double new_rho_minus = currentTree.rho_minus+stepsize*n_dis(gen);
            currentTree.rho_minus = new_rho_minus;

            if (new_rho_minus < 0){
                propLogLik = -INFINITY;
            }else  {
                    // Recalculate log-likelihood
                currentTree.initializeLogLike();
                propLogLik = currentTree.evaluateLogLikeTimesPrior();
            }

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
    }

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


string Sampler::toString(list<pair<pair<int,int>,pair<double,bool>>> L){
    string s = "";
    for (auto it = L.begin(); it!=L.end(); it++ ){
        double score = it->second.first;
        bool trueVal = it->second.second;
        s += to_string(score) + " " + to_string(trueVal) + "\n";
    }
    return s;
}

list<pair<pair<int,int>,pair<double,bool>>> Sampler::meanScores(list<pair<pair<int,int>,pair<double,bool>>> L){
    list<pair<pair<int,int>,pair<double,bool>>> L2;
    list<pair<int,int>> links;

    multimap<pair<int,int>,pair<double,bool>> M;

    for (auto it = L.begin(); it!=L.end(); it++ ){
        pair<int,int> linkId= it->first;


        links.push_back(linkId);
        M.insert(*it);
    }

    links.sort();
    links.unique();
    for(auto lIt = links.begin(); lIt!=links.end(); lIt++){

    pair <multimap<pair<int,int>,pair<double,bool>>::iterator, multimap<pair<int,int>,pair<double,bool>>::iterator> ret;
    ret = M.equal_range(*lIt);

    double sum = 0.0;
    int c = 0;
    bool trueVal = ret.first->second.second;

    for(auto it = ret.first; it != ret.second; ++it){
        sum += it->second.first;
        c++;
    }

    L2.push_back(make_pair(*lIt,make_pair(sum/(double) c, trueVal)));
}
    return L2;
}


/**
 * Identifies and returns a maximum a posteriori probability tree (MAP tree)
 */
Tree Sampler::getMAPTree(){
    double min = -numeric_limits<double>::max();

    auto currentTree = chain.begin();
    Tree * mapTree = nullptr;

    //for (int i = 0; i < (int) likelihoods.size(); ++i){
    for (auto it = likelihoods.begin(); it != likelihoods.end(); ++it) {
        if (min-*it < 0) {
            min = *it;
            mapTree = &(*currentTree);
        }
        ++currentTree;

    }

    mapTree->getRoot()->sortChildren();

    return *mapTree;
}

list<pair<Node *, pair<int, int>>> Sampler::calcSubtreeCred(Node * target_node,list<Node *> valid_trees,int total){


    list<pair<Node*, pair<int, int> > > result;
    list<Node * > targets = target_node->getChildren();

    //For every child
    for (auto target = targets.begin(); target !=targets.end(); ++target) {
        int match = 0;//, total = (int) valid_trees.size();
            list<Node *> valid_subtrees;

            for (auto it = valid_trees.begin(); it != valid_trees.end(); ++it){

                    pair<Node *,bool> x = (*it)->hasEqualSplit(*(*target)->getLeavesP());
                    if (x.second){
                        match++;
                        //Do with x?? Its the root of the next subtree
                        if (x.first != nullptr) {
                            list<Node *> temp =(x.first)->getChildren();
                            valid_subtrees.push_back(x.first);
                        }
                    }
            }

            pair<Node*, pair<int, int> > x_res(*target,pair<int,int>(match,total));
            result.push_back(x_res);


            //Gets subtree results
            result.splice(result.end(), calcSubtreeCred(*target, valid_subtrees, match));

    }
    return result;
}


/**
 *
 *
 * TODO: One occurence of T should be removed from the chain, as otherwise T
 *        will be compared to itself, giving 1 "false" match on the entire tree.
 *        However the effects goes to 0 as the sample size goes to infinity.
 */
vector<pair<int, double>> Sampler::buildCredibilityTree(Tree T){
    vector<pair<int,double >> credibilities(T.getNumNodes());

    //Store a pointer to the root node of every tree from the posterior
    list<Node *> valid_roots;
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        valid_roots.push_back(it->getRoot());
    }

    //Calculate the credibility tree, by calculating the subtree credibilities
    //TODO: calcSubtree should return a vector!
    list<pair<Node *, pair<int, int>>> result = calcSubtreeCred(T.getRoot(), valid_roots,(int) chain.size());

    //Format result for binary search and perserve matches, possible matches
    auto it_vec = credibilities.begin();
    double sampleSize = (double) chain.size();
    for (auto it = result.begin(); it != result.end(); ++it) {
        it_vec->first = it->first->getNodeId(); //Node pointer, for identification
        it_vec->second = it->second.first/sampleSize;
        ++it_vec;
    }
    return credibilities;

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
  /*  int n_sample = 1;
    string filename = "";
    for (auto it = chain.begin(); it != chain.end(); ++it) {
            char buffer [6];
            sprintf (buffer,"%05d", n_sample);
            string n_id = (string) buffer;
            filename = folder + "/tree_sample" + n_id + ".txt";
            it->writeMatlabFormat(filename);
            n_sample++;
    }*/

    // Write likelihood
    filename = folder + "/loglikelihood.txt";
    ofstream out_file(filename);

    for (auto it = likelihoods.begin(); it != likelihoods.end(); ++it){
        out_file << *it << " ";
    }

    // Write Holdout scores to file.
    filename = folder + "/scoresEnsemble.txt";
    ofstream scores(filename);

    list<pair<pair<int,int>,pair<double,bool>>> L;

    for (auto it = chain.begin(); it != chain.end(); ++it) {
        L.splice(L.end(),it->holdoutScores());
    }
    L = meanScores(L);
    scores << toString(L);
    scores.close();

    // Write Holdout scores to file.
    filename = folder + "/scoresMAP.txt";
    scores.open(filename);
    scores << toString(getMapTree().holdoutScores());
    scores.close();


    Tree mapTree = getMAPTree();

    vector<pair<int, double> > cred = buildCredibilityTree(mapTree);
    mapTree.writeJSONFormat(folder + "/mapTree.json",cred);
    mapTree.writeADJlist(folder + "/mapStructure");
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
void Sampler::writeHypers(string folder)
{
    // If folder doesnt exist - create it
    DIR *dir;
    if ((dir = opendir (folder.c_str())) == NULL)
    {
        throw runtime_error("Target directory for writing results not found");
    }

    // Write alpha
    if (sample_alpha)
    {
        string filename = folder + "/hyper_alpha.txt";
        ofstream out_file(filename);

        for (auto it = chain.begin(); it != chain.end(); ++it)
        {
            out_file << it->alpha << " ";
        }
    }


    // Write beta
    if (sample_beta)
    {
        string filename = folder + "/hyper_beta.txt";
        ofstream out_file2(filename);

        for (auto it = chain.begin(); it != chain.end(); ++it)
        {
            out_file2 << it->beta << " ";


        }
    }

    // Write rho_plus
    if (sample_rho_plus)
    {
        string filename = folder + "/hyper_rhop.txt";
        ofstream out_file3(filename);

        for (auto it = chain.begin(); it != chain.end(); ++it)
        {
            out_file3 << it->rho_plus << " ";
        }

    }

    // Write rho_minus
    if (sample_rho_minus)
    {
        string filename = folder + "/hyper_rhom.txt";
        ofstream out_file4(filename);

        for (auto it = chain.begin(); it != chain.end(); ++it)
        {
            out_file4 << it->rho_minus << " ";
        }
    }

}
