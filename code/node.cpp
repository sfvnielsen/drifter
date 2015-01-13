/*
 * node.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: sfvn
 */

#include "node.h"

#include <iostream> // should be removed at some point
#include <numeric> // lets us use cumulative sum
#include <cassert> // for assert statements

using namespace std;

/////////////////////////
// Node class functions //
//////////////////////////

Node::Node(){}

Node::Node(Adj_list * AP) {
    // Trivial constructor.
    parent = nullptr;
    isInternal = true;
    this->AP = AP;
}

Node::Node(Adj_list * AP, int L) {
    // Construct a node with the leaf L
    // This defines a leaf-node
    this->AP = AP;
    parent = nullptr;
    num_internal_nodes = 0;
    leafId = L;
    isInternal = false;
}

list<Node *> Node::getChildren(){
return children;
}


list<int> Node::getLeaves() {
    list<int> leaves;

    if(!isInternal){
        leaves.push_back(leafId);
        return leaves;
    }

    if(!children.empty()) {
        for (list<Node *>::iterator it = children.begin(); it != children.end(); it++) {
            Node * childP = *it;
            leaves.splice(leaves.end(), childP->getLeaves());
        }
    }

    return leaves;
}

Node * Node::getParent() {
    return parent;
}

/**
 * Set the pointer "parent" to a new value.
 */
void Node::setParent(Node * new_parent) {
    parent = new_parent;
}

int Node::getLeafId(){
    return leafId;
}
void Node::setLeafId(int new_id){
    leafId = new_id;
}

/**
* Adds child..
*/
void Node::addChild(Node * childP) {
    // Add a child by:
    //  - Setting the childs parent pointer.
    //  - Adding the childs pointer to the child list.
    childP->setParent(this);
    children.push_back(childP);
}

void Node::removeChild(Node * child) {
    children.remove(child);
}

bool Node::isInternalNode() {
    return isInternal;
}

int Node::getNumInternalNodes(){
    return num_internal_nodes;
}

/**
 * Recursively recalculates the number of internal nodes at each node
 * below the provide node.
 */
int Node::updateNumInternalNodes() {
    int num_internal_below = 0;
    //If the node has children recurse
    if (!this->children.empty()){
        for (auto it = this->children.begin(); it != this->children.end(); it++){
            num_internal_below += (*it)->updateNumInternalNodes();
        }
        num_internal_nodes = num_internal_below+1; //Current node is also internal
        isInternal = true;
    } else { //it is a leaf
        num_internal_nodes = 0;
        isInternal = false;
    }
   return num_internal_nodes; //exited ok
}


string Node::toString() {
    // Building a string representing the tree by printing all of the leaf-Sets

    list<int> leaves = this->getLeaves();
    string s = "leaves: (";
    if(!leaves.empty()) {
        for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++) {
            s += "," + to_string(*it);
        }
    }
    s += ")\n";

    // -- Recurse into children to print the entire subtree.
    // s += "number of children: " + to_string(children.size()) + "\n";
    if(!children.empty()) {
        for (list<Node *>::iterator it = children.begin(); it != children.end(); it++) {
            Node * childP = *it;
            s += childP->toString();
        }
    }
    return s ;
}

/**
* Samples random node from subtree rooted at this node
* - NB! Including root node!!
* - chooses internal nodes with weight 2 and leaves with weight 1
*/
Node * Node::getRandomDescendant() {
    if (isInternal){
        list<Node *> node_list = this->getChildren();
        list<int> subtree_weight;
        for (auto it = node_list.begin(); it!= node_list.end(); ++it ) {
        // Each subtree has weight according to two times the number of
        // internal nodes plus the number of leaves
            subtree_weight.push_back(2*((*it)->getNumInternalNodes())+
                                 (int)((*it)->getLeaves().size()) );

        }
        // DEBUGGING CHECK SUM! ---
        int sum_weight = (int)(this->getLeaves().size())
                          +2*this->getNumInternalNodes();
        int subtree_sum = 2;
        subtree_sum = accumulate(subtree_weight.begin(),subtree_weight.end(),subtree_sum);
        assert(subtree_sum == sum_weight);
        // -------

        list<double> p_vals; // probability vector for each node + root
        for (auto it = subtree_weight.begin(); it!= subtree_weight.end(); ++it) {
            p_vals.push_back((double)*it/sum_weight);
        }

        p_vals.push_back((double)2/sum_weight); // root probability
        node_list.push_back(this);


        // DEBUGGING CHECK!! -------
        double p_val_sum = 0;
        p_val_sum = accumulate(p_vals.begin(),p_vals.end(),p_val_sum);
        assert(abs(p_val_sum-1.0)<1e-12);
        // -------


        Node * sampled_node = multinomialSampling(node_list,p_vals);
        if (sampled_node!=this) { // recurse into chosen subtree
            return sampled_node->getRandomDescendant();
        } else { // else return this node
            return this;
        }

    } else { // if we are at leaf node choose that
        return this;
    }
}

/**
 * Get counts of links and non-links between the pair of children
 */
pair<int, int> Node::getCountsPair(Node * childAP, Node * childBP) {

    list<int> LA = childAP->getLeaves();
    list<int> LB = childBP->getLeaves();

    // Number of possible links
    int nA = (int) LA.size();
    int nB = (int) LB.size();

    int nPossible = nA*nB;

    int nLinks = 0;

    for (list<int>::iterator fst = LA.begin(); fst != LA.end(); fst++) {
        for (list<int>::iterator snd = LB.begin(); snd != LB.end(); snd++) {
            if(AP->isConnected(*fst,*snd)){
                nLinks += 1;
            }
        }
    }

    pair<int, int> result (nLinks,nPossible);

    return result;
}


/**
 * Get counts of links and non-links between all pairs of children
 */
list<pair<int, int>> Node::getCountsAll() {
    list<pair<int, int>> result;
    // Loop through each child
    for (list<Node *>::iterator fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        list<Node *>::iterator nxt = fst;
        // Loop through each child after it in the list
        for (list<Node *>::iterator snd = ++nxt ; snd != children.end(); snd++) {
            result.push_back(this->getCountsPair(*fst, *snd));
        }
    }
    return result;
}

/**
* Log-Beta function
*/
double logbeta(double a, double b){
    return lgamma(a)+lgamma(b)-lgamma(a+b);
}

/**
* Log-Gamma Ratio Function
*/
double lgamma_ratio(double a, double b) {
    return lgamma(a+b)-lgamma(1+b);
}

/**
* Log of Difference (numerically stable)
*/
double log_diff(double a, double b) {
    double maxAB = max(a,b);
    return maxAB + log(exp(a-maxAB)-exp(b-max(a,b)));
}

/**
* Evaluate nodes contribution to likelihood
*/
double Node::evaluateNodeLogLike(double alpha, double beta,
                                 int rho_plus, int rho_minus) {
    double log_like = 0;
    double log_prior = 0;

    list<pair<int,int>> allCountPairs = this->getCountsAll();
    int num_links, num_pos_links; // number of links and possible links

    // Likelihood contribution
    for (list<pair<int,int>>::iterator it = allCountPairs.begin();
         it!=allCountPairs.end(); ++it) {
            num_links = it->first;
            num_pos_links = it->second;
            log_like += logbeta(num_links+rho_plus,
                        num_pos_links-num_links+rho_minus)-
                        logbeta(rho_plus,rho_minus);
    }

    // Prior contribution
    //TODO: Add special case when alpha = 0!
    int num_children = (int) (this->getChildren()).size();
    int num_leaves_total = (int) (this->getLeaves()).size();
    list<int> num_leaves_each_child;
    list<Node *> list_of_children = this->getChildren();

    // Get number of leaves for each child
    for (list<Node *>::iterator it = list_of_children.begin();
             it!= list_of_children.end(); ++it) {
        int num_leaves = (int) ((*it)->getLeaves()).size();
        num_leaves_each_child.push_back(num_leaves);
    }

    // - First term - each child
    for (list<int>::iterator it = num_leaves_each_child.begin();
         it!= num_leaves_each_child.end(); ++it){
        log_prior += lgamma_ratio(*it,-alpha);
    }
    // - Second term
    log_prior += log(alpha+beta) + log(alpha)*(num_children-2)
                -log_diff(lgamma_ratio(num_leaves_total,beta),
                lgamma_ratio(num_leaves_total,-alpha))
                + lgamma(num_children+beta/alpha) - lgamma(2+beta/alpha);
    return log_like+log_prior;
};


/**
* Evaluate entire subtree's contribution to likelihood
*/
double Node::evaluateSubtreeLogLike(double alpha, double beta, int rho_plus
                              , int rho_minus){
    double log_like = 0;

    if (this->isInternal) {
        // First add this nodes contribution
        log_like += this->evaluateNodeLogLike(alpha,beta,rho_plus,rho_minus);
        list<Node *> list_of_children = this->getChildren();
        // Iterate through list of children
        for (list<Node *>::iterator it = list_of_children.begin();
             it!= list_of_children.end(); ++it) {
            // Evaluate each childs subtree-contribution
            log_like += (*it)->evaluateSubtreeLogLike(alpha,beta,rho_plus,rho_minus);
        }
    } else {
        log_like = 0;
    }
    return log_like;
}

/**
* Sampling a random node from a list using a Multinomial distribution
* - p_vals
*/

Node *  multinomialSampling(list<Node *> node_list,list<double> p_vals)  {
    list<double> cumulative_sum(p_vals.size(),0);
    partial_sum(p_vals.begin(),p_vals.end(),cumulative_sum.begin());

    assert(abs(cumulative_sum.back()-1.0 < 1e-12)); // check that p_vals is valid
    assert(node_list.size() == p_vals.size());
    list<Node *>::iterator it_result = node_list.begin();
    Node * result;
    double u = (double)rand()/RAND_MAX; // uniform [0,1] random number
    cout << "Random number generated: " << u << endl;
    for (auto it = cumulative_sum.begin(); it!=cumulative_sum.end(); ++it) { //TODO:!!!  Turn into while loop !! A GULL!?!?
    // finds cumlative interval (correspondng to node a node)
    // in which random number belongs
        if(u>*it) {
            it_result++;
        } else{
            result = *it_result;
            break;
        }
    }
    return result;
}
