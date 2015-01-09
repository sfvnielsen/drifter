/*
 * node.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: sfvn
 */

#include "node.h"

#include <iostream> // should be removed at some point
using namespace std;

/////////////////////////
// Node class functions //
//////////////////////////

Node::Node() {
    // Trivial constructor.
    parent = nullptr;
    isInternal = true;
}

Node::Node(int L) {
    // Construct a node with the leaf L
    // This defines a leaf-node
    parent = nullptr;
    num_internal_nodes = 0;
    leafId = L;
    isInternal = false;
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

void Node::addChild(Node * childP) {
    // Add a child by:
    //  - Setting the childs parent pointer.
    //  - Adding the childs pointer to the child list.
    childP->setParent(this);
    children.push_back(childP);
}

void Node::removeChild(Node * child) {

}

bool Node::isInternalNode() {
    return isInternal;
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
 * Get counts of links and non-links between the pair of children
 */
tuple<int, int> Node::getCountsPair(Node * childAP, Node * childBP) {

    list<int> LA = childAP->getLeaves();
    list<int> LB = childBP->getLeaves();

    // Number of possible links
    int nA = LA.size();
    int nB = LB.size();

    int nPossible = nA*nB;

    int nLinks = 0;

    for (list<int>::iterator fst = LA.begin(); fst != LA.end(); fst++) {
        for (list<int>::iterator snd = LB.begin(); snd != LB.end(); snd++) {
            //result.push_back(this->getCountsPair(*fst, *snd));
        }
    }

    tuple<int, int> result (nLinks,nPossible);

    return result;
}


/**
 * Get counts of links and non-links between all pairs of children
 */
list<tuple<int, int>> Node::getCountsAll() {
    list<tuple<int, int>> result;
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
double loggamma_r(double a, double b) {
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
* Evaluate nodes contribution to likelihood - NOT TESTED!
*/
double Node::evaluateNodeLogLike(double alpha, double beta,
                                 int rho_plus, int rho_minus) {
    double log_like = 0;

    list<tuple<int,int>> allCountPairs = this->getCountsAll();
    int num_links, num_pos_links; // number of links and possible links
    int num_children,num_leaves_total;
    list<int> num_leaves_each_child; //TODO: get somehow!

    // Likelihood contribution
    for (list<tuple<int,int>>::iterator it = allCountPairs.begin();
         it!=allCountPairs.end(); ++it) {
            num_links = get<0>(*it);
            num_pos_links = get<1>(*it);
            log_like += logbeta(num_links+rho_plus,
                        num_pos_links-num_links+rho_minus);
    }

    // Prior contribution
    //TODO: Add special case when alpha = 0!

    // - First term - each child
    for (list<int>::iterator it = num_leaves_each_child.begin();
         it!= num_leaves_each_child.end(); ++it){
        loglike += loggamma_r(*it,-alpha);
        num_leavs_total += *it;
    }
    // - Second term
    loglike += log(alpha+beta) + alpha*(num_children-2)
                -log_diff(loggamma_r(num_leaves_total,beta),
                loggamma_r(num_leaves_total,-alpha))
                + lgamma(num_children+beta/alpha) - lgamma(2+beta/alpha);
    return loglike;
};


/**
* Evaluate entire subtree's contribution to likelihood
*/
double evaluateSubtreeLogLike(double alpha, double beta, int rho_plus, int rho_minus);

