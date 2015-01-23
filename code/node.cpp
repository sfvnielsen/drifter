/*
 * Node.cpp
 *
 *  Created on: Jan 9, 2015 as part of parmugit project
 *      Authors: Jesper, Julian and Søren
 *
 */

#include "node.h"
#include "tree.h"

#include <iostream>
#include <numeric>  // lets us use cumulative sum
#include <cassert> // for assert statements
#include <random> // C++11 random generator
#include <cmath> //lgamma and other math functions


using namespace std;

/** Constructors
 *
 */

Node::Node(){}

/**
 * Initialises a empty node beloning to an already constructed tree
 */
Node::Node(Tree * tP): treeP(tP){
    // Trivial constructor.
    parentP = nullptr;
}

/**
 * Initialises a node in a tree with a specified id (which should be unique)
 * Leafs ids should be in range 0 to N
 * Internal node ids should be negative.
 */
Node::Node(Tree * tP, int L): treeP(tP) {
    parentP = nullptr;
    num_internal_nodes = 0;
    nodeId = L;

}

/**
* Recursively copy nodes from another tree.
*/
void Node::copyFrom(Tree * tP, Node const & old_node){
    //Copies old fields to new node
    treeP = tP;
    nodeId = old_node.nodeId;
    leaves = old_node.leaves;
    num_internal_nodes = old_node.num_internal_nodes;
    loglikelihood_cont = old_node.loglikelihood_cont;
    
    //Recurses through the children of the current node
    for (auto it = old_node.children.begin(); it != old_node.children.end(); it++) {
        // add new node
        Node * new_child = treeP->addNode();
        // add as child
        this->addChild(new_child);
        // recurse
        new_child->copyFrom(treeP, **it);
    }
}

/**
 * Returns a pointer to the nodes parent
 */
Node * Node::getParent() {
    return parentP;
}

/**
 * Changes the parent of the node
 */
void Node::setParent(Node * new_parentP) {
    parentP = new_parentP;
}

/**
* Returns node ID
*/
int Node::getNodeId(){
    return nodeId;
}
/**
 * Changes the node id to the input value
 */
void Node::setNodeId(int new_id){
    nodeId = new_id;
}

/**
 * Returns a list of pointers to the leaf nodes of the current node
 */
list<int> * Node::getLeaves() {
    assert(isInternalNode() == ((nodeId < 0) && children.size() >0));
    return &leaves;
}

/**
 * Returns the number of internal nodes at this node
 * IFF leaf node:       always zero (0) internal nodes
 * IFF internal node:   number of internal nodes below this + 1
 */
int Node::getNumInternalNodes(){
    return num_internal_nodes;
}

/**
 * Returns a list of pointers to all the current nodes children
 */
list<Node *> Node::getChildren(){
    return children;
}

/**
 * Replaces the current children Node * list with the new_children list
 *      NOTE: This function simply changes the list, it does not update
 *            anything further!
 */
void Node::setChildren(list<Node *> new_children){
    children = new_children;
}

/**
 * Add a new child to the current nodes children list, and changes the parent
 *  of the added node.
 */
void Node::addChild(Node * new_childP) {
    new_childP->setParent(this);
    children.push_back(new_childP);
}

/**
 * Removes the provided child as a child the current node. And maintains a 
 *  valid tree structure by collapsing the node if it only has 1 child left
 */
bool Node::removeChild(Node * child) {
    bool collapsed = false;
    children.remove(child);
    
    //IFF: The current node only have two children, it is collapsed as every
    //     internal node must have atleast 2 children)
    if(1==(int) children.size()){
        //If the node is the tree root, the single child should be the new root
        if(this == (treeP->getRoot()) ){
            children.front()->setParent(nullptr);
            treeP->setRootP(children.front());
            treeP->removeNode(this);
            collapsed = true;
        }else{
        //Collaps this node, and adds its child to the grandparent
            parentP->addChild(children.front());
            parentP->removeChild(this);
            treeP->removeNode(this);
            collapsed = true;
        }
    }
    return collapsed;
}

/**
* Recursively samples a random node from subtree rooted at this node, if called
*  on the root a random node from the entire tree is sampled.
*  
* Internal nodes are sampled with weight two (2), and leaf nodes are sampled
*    with weight one (1) . 
*
* The reason for this weighting is that at internal nodes a scion can be inserted
*  as either a child or a sibling, where as at leaf nodes a scion can only be
*  inserted as a sibling (leaf nodes have no children).
*/
Node * Node::getRandomDescendant() {
    
    if (!isInternalNode()) {
        return this;
    }
    
    //If it is an internal node
    list<Node *> node_list = getChildren();
    list<int> subtree_weight;
    for (auto it = node_list.begin(); it!= node_list.end(); ++it ) {
        // Each subtree has weight according to two times the number of
        // internal nodes plus the number of leaves
        assert((*it)->getNumInternalNodes() == 0 || (*it)->isInternalNode());
        assert((*it)->getNumInternalNodes()> 0 || !(*it)->isInternalNode());
        subtree_weight.push_back(2*((*it)->getNumInternalNodes())+
                                 (int)(((*it)->getLeaves())->size()) );
    }
    int sum_weight = (int)((this->getLeaves())->size())
    +2*this->getNumInternalNodes();
    list<double> p_vals; // probability vector for each node + root
    for (auto it = subtree_weight.begin(); it!= subtree_weight.end(); ++it) {
        p_vals.push_back((double)*it/sum_weight);
    }
    
    p_vals.push_back((double)2/sum_weight); // root probability
    node_list.push_back(this);
    
    // assert that p_vals sums to 1
    double p_val_sum = 0;
    p_val_sum = accumulate(p_vals.begin(),p_vals.end(),p_val_sum);
    assert(abs(p_val_sum-1.0)<1e-12);
    
    // Sample one of the nodes
    Node * sampled_node = multinomialSampling(node_list,p_vals);
    if (sampled_node!=this) { // recurse into subtree rooted at node
        return sampled_node->getRandomDescendant();
    } else { // else return this node
        return this;
    }
}

/**
* Sampling a random node from a list using a Multinomial distribution
* - p_vals
*/

Node *  multinomialSampling(list<Node *> node_list,list<double> p_vals)  {
    list<double> cumulative_sum(p_vals.size(),0);
    
    // finding cumulative sum (partial sum) of p_vals
    partial_sum(p_vals.begin(),p_vals.end(),cumulative_sum.begin());

    assert(abs(cumulative_sum.back()-1.0 < 1e-12)); // check that p_vals is valid
    assert(node_list.size() == p_vals.size());

    list<Node *>::iterator it_result = node_list.begin();
    list<double>::iterator it = cumulative_sum.begin();
    std::random_device rd;
    std::mt19937 random_generator(rd());
    std::uniform_real_distribution<> dis(0, 1);
    double u = dis(random_generator);
    //double u = (double)rand()/RAND_MAX; // uniform [0,1] random number

    while (u>*it) { // finds interval (node) that u corresponds to
                    // i.e. the first time u is smaller than the cumulative element
        it_result++;
        it++;
    }
    return *it_result;
}

/**
 Update leaves list at each node in tree
*/
void Node::updateLeaves(){
    leaves.clear();

    if(!isInternalNode()){
        leaves.push_back(nodeId);
    }else{
        for (auto it = children.begin(); it != children.end(); it++) {
            Node * childP = *it;
            childP->updateLeaves();
            list<int> childLeaves = *(childP->getLeaves());
            assert(childLeaves.size()>0);
            leaves.splice(leaves.end(), childLeaves);
            leaves.sort();
            leaves.unique();
        }
    }
    assert(isInternalNode() == ((nodeId < 0) && children.size() >0));
    assert(leaves.size()>0);
}

/**
 * Recursively recalculates the number of internal nodes at each node
 * below the provide node.
 */
int Node::updateNumInternalNodes() {
    int num_internal_below = 0;
    //If the node has children recurse
    if (!children.empty()){ //Then it is an internal node
        for (auto it = children.begin(); it != children.end(); it++){
            num_internal_below += (*it)->updateNumInternalNodes();
        }
        num_internal_nodes = num_internal_below+1; //Current node is also internal
    } else { //it is a leaf
        num_internal_nodes = 0;
    }
   return num_internal_nodes; //exited ok
}

/** Log-Like times prior calculation of node
*/

/**
* Evaluate nodes contribution to likelihood
*/
double Node::evaluateNodeLogLike(double alpha, double beta,
                                 int rho_plus, int rho_minus) {
    //Never evaluate on leaf nodes
    if (! isInternalNode()) {
        loglikelihood_cont = 0.0;
        return 0.0;
    }
    
    double log_like = 0.0;
    double log_prior = 0.0;

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
    int num_leaves_total = (int) (this->getLeaves())->size();
    list<int> num_leaves_each_child;
    list<Node *> list_of_children = this->getChildren();

    // Get number of leaves for each child
    for (list<Node *>::iterator it = list_of_children.begin();
             it!= list_of_children.end(); ++it) {
        int num_leaves = (int) (*it)->getLeaves()->size();
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
    // cout << "Like: " << log_like << "  Prior: " << log_prior << endl;
    
    assert(!isinf(log_like) );
    assert(!isinf(log_prior) );
    loglikelihood_cont = log_like+log_prior;
    return log_like+log_prior;
};


/**
* Evaluate entire subtree's contribution to likelihood
*/
double Node::evaluateSubtreeLogLike(double alpha, double beta, int rho_plus
                              , int rho_minus){
    double log_like = 0.0;

    if (this->isInternalNode()) {
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
        log_like = 0.0;
    }
//    loglikelihood_cont = log_like;
    return log_like;
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
 * Get counts of links and non-links between the pair of children
 */
pair<int, int> Node::getCountsPair(Node * childAP, Node * childBP) {

    list<int> * LA = childAP->getLeaves();
    list<int> * LB = childBP->getLeaves();

    // Number of possible links
    int nA = (int) LA->size();
    int nB = (int) LB->size();

    int nPossible = nA*nB;

    int nLinks = 0;

    Adj_list * AP = treeP->getAdjacencyListP();

    // Loop through all all combinations of leaves and check if they are connected
    for (list<int>::iterator fst = LA->begin(); fst != LA->end(); fst++) {
        for (list<int>::iterator snd = LB->begin(); snd != LB->end(); snd++) {
            if(AP->isConnected(*fst,*snd)){
                nLinks += 1;
            }
        }
    }

    pair<int, int> result (nLinks,nPossible);
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
 Equality (Bool) operations
*/

bool Node::operator==( const Node &rhs ) const {
    return nodeId == rhs.nodeId;
}

/**
 * Tests if two subtrees are equal, defined as when all internal nodes (below) in the
 *  original tree have the same children as the internal nodes (below) in the "copy" tree.
 *
 * Also symmetric subtree structure is allowed, as long as each node furfills the
 *  same leaves requirement.
 *
 * !!! Should not be run on large networks !!!
 */
bool Node::isEqualSubtree(Node * copy_node){

    list<int> leavesOriginal  = *getLeaves(),
    leavesCopy = * copy_node->getLeaves();

    //If the number of leaves are diffent, they are never a match
    if (leavesCopy.size() != leavesOriginal.size()) {
        return false;
    } else {
        //Computes how many of the leaves between copy and original are equal
        int num_equal = 0;
        for (auto it = leavesOriginal.begin(); it != leavesOriginal.end(); it++) {
            for (auto it2 = leavesCopy.begin(); it2 != leavesCopy.end(); it2++) {
                if (*it == *it2) {
                    num_equal++;
                }
            }
        }

        // iff the following check passes, are the leaves identical
        if (num_equal == (int) leavesOriginal.size()) {

            //Now the children must be comparred and as the trees can be symmetric
            // each possible combination of copy and original children are compared.
            int num_child_identical = 0;
            for (auto it = children.begin(); it != children.end(); it++) {
                for (auto it2 = copy_node->children.begin(); it2 != copy_node->children.end(); it2++) {

                    //Each check starts a recursion, and at maximum one can be true
                    // for each iteration of the outer for-loop.
                    if ((*it)->isEqualSubtree( (* it2) ) ) {
                        num_child_identical++;
                    }
                }


            }
            // iff the following check passes, are the subtrees identical
            if (num_child_identical == (int) children.size()) {
                return true;
            }


        }


    }
    return false;
}

/**
 isInternal bool operation
*/
bool Node::isInternalNode() {
    return !children.empty();
}

/** Printing
*/
string Node::toString() {
    // Building a string representing the tree by printing all of the leaf-Sets

    list<int> leaves = *(this->getLeaves());
    string s = "Node: " +  to_string(getNodeId()) +"; Num_internal: ("+to_string(getNumInternalNodes())+ "); Leaves: (";
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
 * Sets number of internal nodes
 */
void Node::setNumInternalNodes(int new_num_internal){
    num_internal_nodes = new_num_internal;
}

bool Node::isNCA(Node * targetP){

//    if (this != treeP->getRoot()) {
        int target = targetP->getLeaves()->front();

        for (auto it = leaves.begin(); it != leaves.end(); it++) {
            if (*it == target){
                return true;
            }
        }
  //  } else {
    //    return true;
   // }
    return false;
}

bool Node::isSubsetOf(Node * targetP){
    
    if (leaves.size() > targetP->leaves.size()) {
        return false;
    }
    
    for (auto it = leaves.begin(); it != leaves.end(); it++) {
        bool element_found = false;
        for (auto it2 = targetP->leaves.begin(); it2 != targetP->leaves.end(); ++it2) {
            if (*it == * it2) {
                element_found = true;
            }
        }
        if (!element_found) {
            return false;
        }
    }
    return true;
}

double Node::getLogLikeContribution(){
    return loglikelihood_cont;
}
void Node::setLogLikeContribution(double new_contribution){
    loglikelihood_cont = new_contribution;
}
