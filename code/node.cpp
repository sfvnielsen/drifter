/**
 *  Project:        Parmugit
 *  Class:          Node
 *  Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
 *  Affiliation:    Technical University of Denmark
 *  Date:           January 2015
 */


#include "node.h"
#include "tree.h"

#include <iostream>
#include <algorithm>
#include <numeric>  // lets us use cumulative sum
#include <cassert> // for assert statements
#include <random> // C++11 random generator
#include <cmath> //lgamma and other math functions
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

/**
 * Constructors
 */
Node::Node(){}

/**
 * Initialises a empty node beloning to an already constructed tree
 */
Node::Node(Tree * tP): treeP(tP){
    // Trivial constructor.
    parentP = nullptr;
    logPrior = 0.0;
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
    logPrior = 0.0;

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
    pairLogLikeCont = old_node.pairLogLikeCont;
    logPrior = old_node.logPrior;

    //Recurses through the children of the current node
    for (auto it = old_node.children.begin(); it != old_node.children.end(); it++) {
        // add new node
        Node * new_child = treeP->addNode();
        // add as child
        addChild(new_child);
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
vector<int> * Node::getLeavesP() {
    assert(isInternalNode() == ((nodeId < 0) && children.size() >0));
    return &leaves;
}

void Node::setLeaves(vector<int> new_leaves){
    leaves = new_leaves;
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
 * Sets number of internal nodes
 */
void Node::setNumInternalNodes(int new_num_internal){
    num_internal_nodes = new_num_internal;
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
    //assert(isLogLikeCacheCorrect());
    assert(isInternalNode());
    assert(children.size()>=2);

    //IFF: The current node only have two children, it is collapsed as every
    //     internal node must have atleast 2 children)
    if(2==(int) children.size()){
        //If the node is the tree root, the single child should be the new root
        if(this == (treeP->getRoot()) ){
            children.remove(child);
            assert((int) children.size()==1);
            children.front()->setParent(nullptr);
            treeP->setRootP(children.front());
            treeP->removeNode(this);
            return true;
        }else{
        //Collaps this node, and adds its child to the grandparent
            children.remove(child);
            assert((int)children.size()==1);
            Node * remainingChild = children.front();

//            parentP->removeChildCached(this);
//            parentP->addChildCached(remainingChild);
            parentP->replaceChild(this,remainingChild);

            treeP->removeNode(this);
            return true;
        }
    }else{
        removeChildCached(child);
        return false;
    }

    //assert(isLogLikeCacheCorrect());
}

bool Node::removeChildCached(Node * childP) {
    //assert(isLogLikeCacheCorrect());

    int N = (int) children.size();
    --N;//A node is removed

    vector<double> new_pairLogLike;
    new_pairLogLike.reserve(N*(N-1)/2);

    auto it = pairLogLikeCont.begin();

    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        auto nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            if((*fst) != childP && (*snd) != childP){
                new_pairLogLike.push_back(*it);
                // Works: new_pairLogLike.push_back(evaluatePairLogLike(*fst,*snd));
            }
            ++it;
        }
    }
    pairLogLikeCont = new_pairLogLike;
    children.remove(childP);

    //updateAllPairsLogLike();

    assert((int) children.size() == N);
    //assert(isLogLikeCacheCorrect());
    return false;
}

void Node::addChildCached(Node * childP) {

    //assert(isLogLikeCacheCorrect());

    children.push_back(childP);
    childP->setParent(this);

    int N = (int) children.size();

    vector<double> new_pairLogLike;
    new_pairLogLike.reserve(N*(N-1)/2);

    auto it = pairLogLikeCont.begin();

    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        auto nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            if(*fst == childP){ // der skal tilføjes noget
                double L = evaluatePairLogLike(childP, *snd);
                new_pairLogLike.push_back(L);
            }else if(*snd ==childP){ // der skal tilføjes noget
                double L = evaluatePairLogLike(*fst, childP);
                new_pairLogLike.push_back(L);
            }else{
                new_pairLogLike.push_back(*it);
                ++it;
            }
        }
    }
    pairLogLikeCont = new_pairLogLike;

    //assert(isLogLikeCacheCorrect());
}

//Kald på parent af nodeToReplace
void Node::replaceChild(Node * nodeToReplace, Node * new_node) {
    //assert(isLogLikeCacheCorrect());
    //Erstat
    assert(this != nullptr);
    auto it = pairLogLikeCont.begin();
    //Updatere loglike contribution
    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        auto nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            if(*snd == nodeToReplace) { // der skal tilføjes noget
                (*it) = evaluatePairLogLike(*fst, new_node);
            } else if (*fst == nodeToReplace) {
                (*it) = evaluatePairLogLike(new_node, *snd);
            } else {
                //No change in parameters/loglike
            }
            ++it;
        }
    }
    //Erstat nodeToReplace med new_node i children listen
    auto itNode = find(children.begin(), children.end(), nodeToReplace);
    assert(itNode != children.end());
    *itNode = new_node;
    new_node->setParent(this);

    //assert(isLogLikeCacheCorrect());
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
    assert(isNumInternalNodesCorrect());

    if (!isInternalNode()) {
        return this;
    }

    //Otherwise it is an internal node
    list<Node *> node_list = getChildren();
    vector<int> subtree_weight;

    // Each child is a subtree and it weight is calculated and stored for each subtree (child)
    for (auto it = node_list.begin(); it!= node_list.end(); ++it ) {
        subtree_weight.push_back(2*((*it)->getNumInternalNodes())+
                                 (int)(((*it)->getLeavesP())->size()) );
        //Checks for consistency in isInternal
        assert((*it)->getNumInternalNodes() == 0 || (*it)->isInternalNode());
        assert((*it)->getNumInternalNodes()> 0 || !(*it)->isInternalNode());
    }
    //The weights summes to the weight of the subtree rooted at the current node
    int sum_weight = (int)((getLeavesP())->size())+
                            2*getNumInternalNodes();
    //The weights are normalised to a probability and store in vector p_vals
    vector<double> p_vals;
    for (auto it = subtree_weight.begin(); it!= subtree_weight.end(); ++it) {
        p_vals.push_back((double)*it/sum_weight);
    }

    p_vals.push_back((double)2/sum_weight); // Root probability added
    node_list.push_back(this);

    #ifndef NDEBUG
    // assert that p_vals sums to 1
    double p_val_sum = 0;
    p_val_sum = accumulate(p_vals.begin(),p_vals.end(),p_val_sum);
    assert(abs(p_val_sum-1.0)<1e-12);
    #endif // NDEBUG

    // Sample from this distribution
    Node * sampled_node = multinomialSampling(node_list,p_vals);
    if (sampled_node!=this) {
         //If the current node is not selected, recurse into the subtree selected
        return sampled_node->getRandomDescendant();
    } else { // else return this node
        return this;
    }
}

/**
 * Sampling a random node from a list using a Multinomial distribution
 * @param node_list (Sample space, nodes)
 * @param p_vals    (Probability for sampling the corresponding node)
 */
Node *  multinomialSampling(list<Node *> node_list,vector<double> p_vals)  {
    vector<double> cumulative_sum(p_vals.size(),0);

    // Finding cumulative sum (partial sum) of p_vals
    partial_sum(p_vals.begin(),p_vals.end(),cumulative_sum.begin());

    // check that p_vals is valid
    assert(abs(cumulative_sum.back()-1.0) < 1e-12);
    assert(node_list.size() == p_vals.size());

    list<Node *>::iterator it_result = node_list.begin();
    vector<double>::iterator it = cumulative_sum.begin();
    //Samples a random Uniform[0,1] number
    std::random_device rd;
    std::mt19937 random_generator(rd());
    std::uniform_real_distribution<> dis(0, 1);
    double u = dis(random_generator); //Alternatively (double)rand()/RAND_MAX

    // The first time u is smaller than the cumulative element, is the sampled node
    while (u>*it) {
        it_result++;
        it++;
    }
    return *it_result;
}

/**
 * Update leaves at each node in tree, by asking the nodes children for
 *  their leaves and combining them as the leaves list for the current node.
 *
 * Recurses through all nodes below.
 */
void Node::updateLeaves(){
    leaves.clear();

    if(!isInternalNode()){ //Leaf node
        leaves.push_back(nodeId);
        assert(!leaves.empty());
    }else{
        for (auto it = children.begin(); it != children.end(); it++) {
            Node * childP = *it;
            childP->updateLeaves(); //Make sure the childs leafs are updated

            //For each child copy the child list to this node.
            // NOTE: Using splices removes the elements from the list.
            vector<int> childLeaves = *(childP->getLeavesP());
            assert(childLeaves.size()>0);
            //Combines a sorted list!
            addLeaves(childLeaves);
        }
    }
    assert(isInternalNode() == ((nodeId < 0) && children.size() >0));
    assert(leaves.size()>0);
}

/**
 * Adds the elements in add_leaves to leaves of the node. Both must be sorted.
 */
void Node::addLeaves(std::vector<int> add_leaves){
    vector<int> old_leaves = leaves;
    leaves=vector<int>((int) leaves.size()+ (int) add_leaves.size());
    merge(old_leaves.begin(), old_leaves.end(), add_leaves.begin(), add_leaves.end(), leaves.begin());
}

/**
 * Removes leaves in rem_leaves from the leaves of the Node. Both rem_leaves and
 *  leaves must be sorted. O(|rem_leaves|+|leaves|)
 */
void Node::removeLeaves(std::vector<int> rem_leaves){
    vector<int> old_leaves = leaves;
    leaves=vector<int>((int) leaves.size()- (int) rem_leaves.size());
    //Copies elements that are in old_leaves but not in rem_leaves
    set_difference(old_leaves.begin(), old_leaves.end(), rem_leaves.begin(), rem_leaves.end(), leaves.begin());
}


/**
 * Recursively recalculates the number of internal nodes at each node
 * below the provide node.
 */
int Node::updateNumInternalNodes() {
    int num_internal_below = 0;
    //If the node has children, recurse into them.
    if (!children.empty()){
        for (auto it = children.begin(); it != children.end(); it++){
            num_internal_below += (*it)->updateNumInternalNodes();
        }
        //The current node has internal nodes 1 + number of internal nodes below it
        num_internal_nodes = num_internal_below+1;
    } else { //It is a leaf
        num_internal_nodes = 0;
    }

   return num_internal_nodes;
}

/**
 * Recalculate number of internal nodes and check if it was correct
 */
bool Node::isNumInternalNodesCorrect() {
    int n = getNumInternalNodes();
    updateNumInternalNodes();
    return n == num_internal_nodes;
}

/**
 * Evaluate nodes contribution to log (likelihood times prior)
 *   (non-normalised posterior)
 *
 * All node parameter pairs for this nodes children are calculated,
 *  i.e links and possible links between two nodes (subtrees).
 *
 * The the log(likelihood times prior) is then calculated with these pairs.
 */
double Node::evaluateNodeLogLike() {

    //Evaluation on leaves are always zero (0.0)
    if (! isInternalNode()) {
        return 0.0;
    }

    double log_like = 0.0;

    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        list<Node *>::iterator nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            log_like += evaluatePairLogLike(*fst, *snd);
        }
    }

    double log_prior = evaluateLogPrior();
    return log_like+log_prior;
};

double Node::evaluateLogPrior(){

    if (!isInternalNode()) {
        return 0.0;
    }

    double alpha = treeP->alpha;
    double beta = treeP->beta;

    // Prior contribution for each node
    double log_prior = 0.0;

    int num_children = (int) (getChildren()).size();
    int num_leaves_total = (int) (getLeavesP())->size();
    vector<int> num_leaves_each_child;
    list<Node *> list_of_children = getChildren();

    // Get number of leaves for each child
    for (auto it = list_of_children.begin();
         it!= list_of_children.end(); ++it) {
        int num_leaves = (int) (*it)->getLeavesP()->size();
        num_leaves_each_child.push_back(num_leaves);
    }

    // Special case if alpha is zero
    if (alpha < 1e-12){
        // - First term in prior contribution - each child
        for (auto it = num_leaves_each_child.begin();
             it!= num_leaves_each_child.end(); ++it){
            log_prior += lgamma(*it);
        }
        // - Second term in prior contibution
        log_prior += log(beta)*(num_children-1)
        -log_diff(lgamma_ratio(num_leaves_total,beta),
                  lgamma(num_leaves_total));

    }else{
        // - First term in prior contribution - each child
        for (auto it = num_leaves_each_child.begin();
             it!= num_leaves_each_child.end(); ++it){
            log_prior += lgamma_ratio(*it,-alpha);
        }
        // - Second term in prior contibution
        log_prior += log(alpha+beta) + log(alpha)*(num_children-2)
        -log_diff(lgamma_ratio(num_leaves_total,beta),
                  lgamma_ratio(num_leaves_total,-alpha))
        + lgamma(num_children+beta/alpha) - lgamma(2+beta/alpha);
    }
    assert(isfinite(log_prior));


    return log_prior;
}

double Node::evaluatePairLogLike(Node * childAP, Node * childBP){
    int num_links, num_non_links;
    double log_like;

    double rho_plus = treeP->rho_plus;
    double rho_minus = treeP->rho_minus;

    pair<int, int> counts = getObservedCountsPair(childAP,childBP);

    num_links = counts.first;
    num_non_links = counts.second;
    log_like =  logbeta(num_links+rho_plus,
                        num_non_links+rho_minus)
    -logbeta(rho_plus,rho_minus);

    return log_like;
}


/**
 * Evaluate entire subtree's contribution to log( likelihood times prior),
 * by recursing through the subtree.
 *
 * NOTE: Only used for initialisation, as the values are cached at each node.
 */
double Node::evaluateSubtreeLogLike(){
    double log_like = 0.0;
    if (isInternalNode()) {
        // First add this nodes contribution
        log_like += evaluateNodeLogLike();
        list<Node *> list_of_children = getChildren();
        // Iterate through list of children
        for (auto it = list_of_children.begin();
             it!= list_of_children.end(); ++it) {
            // Evaluate each childs subtree-contribution
            log_like += (*it)->evaluateSubtreeLogLike();
        }
    } else {
        log_like = 0.0;
    }

    return log_like;
}

/**
 * Get counts of links and non-links between the pair of children
 */
pair<int, int> Node::getObservedCountsPair(Node * childAP, Node * childBP) {

    vector<int> * LAP = childAP->getLeavesP();
    vector<int> * LBP = childBP->getLeavesP();

    Adj_list * adjacency_list = treeP->getAdjacencyListP();

    return adjacency_list->getCounts(LAP, LBP);
}

/**
 * Get counts of links and non-links between the pair of children
 */
pair<int, int> Node::getUnobservedCountsPair(Node * childAP, Node * childBP) {

    vector<int> * LAP = childAP->getLeavesP();
    vector<int> * LBP = childBP->getLeavesP();

    Adj_list * adjacency_list = treeP->getAdjacencyListP();

    return adjacency_list->getUnknownCounts(LAP, LBP);
}

std::list<std::pair<std::pair<int,int>,bool>> Node::getUnobservedLinksPair(Node * childAP, Node * childBP) {

    vector<int> * LAP = childAP->getLeavesP();
    vector<int> * LBP = childBP->getLeavesP();

    Adj_list * adjacency_list = treeP->getAdjacencyListP();

    return adjacency_list->getUnknownLinks(LAP, LBP);
}


/**
 * Get cached log likelihood contribution
 */
double Node::getLogLike(){
    // the node pairs likelihood contribution
    double log_like = 0.0;
    for(auto it=pairLogLikeCont.begin(); it!=pairLogLikeCont.end(); ++it){
        log_like += *it;
    }
    assert(isfinite(log_like));

    // TODO: use accumulate, "return accumulate(pairLogLikeCont.begin(),pairLogLikeCont.end(),0);"
    return log_like;
}

/**
 * Get cached log prior
 */
double Node::getLogPrior(){
    assert(isfinite(logPrior));
    return logPrior;
}

/**
 * Update cached node prior
 */
void Node::updateNodeLogPrior(){
    logPrior = evaluateLogPrior();
    assert(isfinite(logPrior));
}

/**
 * Update all cached pairs likelihood.
 */
void Node::updateAllPairsLogLike(){
    // clear out all values
    pairLogLikeCont.clear();

    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        auto nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            double L = evaluatePairLogLike(*fst, *snd);
            pairLogLikeCont.push_back(L);
        }
    }
}
/**
 * Update cached pairs related to a child.
 */
void Node::updateChildPairsLogLike(Node * childP) {

    #ifndef NDEBUG
        int N = (int) children.size();
    #endif
    assert((int) pairLogLikeCont.size() == (N*(N-1))/2);

    auto it = pairLogLikeCont.begin();
    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        auto nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            if( (*fst == childP || *snd == childP) ) {
                double L = evaluatePairLogLike(*fst, *snd);
                (*it) = L;
            }
            ++it;
        }
    }

}

bool Node::isLogLikeCacheCorrect(){
    double log_like = 0.0;

    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        list<Node *>::iterator nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            log_like += evaluatePairLogLike(*fst, *snd);
        }
    }
    return log_like == getLogLike();
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
 * Equality (Bool) operations
 */
bool Node::operator==( const Node &rhs ) const {
    return nodeId == rhs.nodeId;
}

/**
 * Tests if two subtrees are equal, defined as when all internal nodes (below) the
 *  original tree have the same children as the internal nodes (below)
 *  in the "copy" tree.
 *
 * Also symmetric subtree structure is allowed, as long as each node furfills the
 *  same leaves requirement.
 *
 * !!! Should not be run on large networks !!!
 */
bool Node::isEqualSubtree(Node * copy_node){

    vector<int> leavesOriginal  = *getLeavesP(),
    leavesCopy = * copy_node->getLeavesP();

    //If the number of leaves are diffent, they are never a match
    if (leavesCopy.size() != leavesOriginal.size()) {
        return false;
    } else {
        //Computes how many of the leaves between copy and original are equal
        int num_equal = 0;

        //TODO use algorithm package to compare two sorted lists
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
 * Compares if any of this node's children has an identical split to the target.
 *
 * Returns:
 *  1. A pointer to the child or nullptr if the child is not found or it's a leaf
 *  2. A bool indicating if there is an equal child.
 */
pair<Node *, bool> Node::hasEqualSplit(std::vector<int> targetLeaves){

    if (children.size() == 0 && targetLeaves.size() == 1 && leaves[0]==targetLeaves[0]) {
        return pair<Node *, bool>(nullptr,true);
    }

    //Compare the target split to each of this nodes spilt, which is equivivalent
    // to comparing this nodes childrens (sorted) leaves list
    for (auto it_child =children.begin(); it_child !=children.end(); ++it_child) {
        vector<int> child_leaves = *(*it_child)->getLeavesP();

        if (targetLeaves.size() == child_leaves.size()) {

            //Compares split, sorted leaves list allow linear time comparison
            auto it_t = targetLeaves.begin();

            bool isEqual = true;
            //If any two elements are not equal, then the splits are not equal
            for (auto it = child_leaves.begin(); it!=child_leaves.end(); ++it) {
                if (*it != *it_t) {
                    isEqual = false;
                    break;
                }
                ++it_t;

            }
            //If one child has an equal split, no other children can have one
            if (isEqual) {
                return pair<Node*, bool>(*it_child,true);
            }
        }
    }

    return pair<Node*, bool>(nullptr,false);
}


/**
 * Sort the children by the size of their leaves. Default is in accending order.
 */
void Node::sortChildren(){sortChildren(true);};

/**
 * Sort the children by the size of their leaves. In accending or descending order.
 */
void Node::sortChildren(bool ascending){
    if (children.size()>1) {

        //Sorting using lambda functions
        if (ascending) {
            children.sort([](Node * a,Node * b)->bool{
                return a->getLeavesP()->size() < b->getLeavesP()->size();
            });
        } else {
            children.sort([](Node * a,Node * b)->bool{
                return a->getLeavesP()->size() > b->getLeavesP()->size();
            });

        }
        //Recurse
        for (auto it = children.begin(); it != children.end(); ++it) {
            (*it)->sortChildren(ascending);
        }
    }

}

/**
 * Checks whether a node an internal node, by looking at its children or lack of
 */
bool Node::isInternalNode() {
    return !children.empty();
}


pair<int,int> Node::predictionResults(){
    int correct = 0;
    int wrong = 0;
    pair<int, int> knownCounts;
    pair<int, int> unknownCounts;

    int rho_plus = treeP->rho_plus;
    int rho_minus = treeP->rho_minus;

    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        auto nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            knownCounts = getObservedCountsPair(*fst,*snd);
            unknownCounts = getUnobservedCountsPair(*fst,*snd);
            bool prediction = 0.5 < (double) (rho_plus + knownCounts.first)/(rho_plus+knownCounts.first+rho_minus+knownCounts.second);

            if(prediction){
                correct += unknownCounts.first;
                wrong += unknownCounts.second;
            }else{
                correct += unknownCounts.second;
                wrong += unknownCounts.first;
            }
        }
    }

    pair<int,int> result (correct,wrong);
    return result;
}

list<pair<pair<int,int>,pair<double,bool>>> Node::holdoutScores(){
    list<pair<pair<int,int>,pair<double,bool>>> L;
    pair<int, int> knownCounts;
    list<pair<pair<int,int>,bool>> unknownLinks;

    int rho_plus = treeP->rho_plus;
    int rho_minus = treeP->rho_minus;

    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        auto nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            knownCounts = getObservedCountsPair(*fst,*snd);
            unknownLinks = getUnobservedLinksPair(*fst,*snd);

            double score = (double) (rho_plus + knownCounts.first)/(rho_plus+knownCounts.first+rho_minus+knownCounts.second);

            for(auto it = unknownLinks.begin(); it!= unknownLinks.end(); it++){
                pair<int,int> linkId = it->first;
                bool trueValue = it->second;
                L.push_back(make_pair(linkId,make_pair(score,trueValue)));
            }
        }
    }
    return L;
}

/**
 * Printing by recursing through the subtree root at this node
 */
string Node::toString() {
    // Building a string representing the tree by printing all of the leaf-Sets
    vector<int> leaves = *(getLeavesP());
    string s = "Node: " +  to_string(getNodeId()) +"; Num_internal: ("+to_string(getNumInternalNodes())+ "); Leaves: (";
    if(!leaves.empty()) {
        for (auto it = leaves.begin(); it != leaves.end(); it++) {
            s += "," + to_string(*it);
        }
    }
    s += ")\n";

    // -- Recurse into children to print the entire subtree.
    // s += "number of children: " + to_string(children.size()) + "\n";
    if(!children.empty()) {
        for (auto it = children.begin(); it != children.end(); it++) {
            Node * childP = *it;
            s += childP->toString();
        }
    }
    return s ;
}


string Node::toGexf(int indent){
    string pad = string(indent*4, ' ');
    string s = pad + "<node id=\"" + to_string(nodeId);
    s += "\">";
    if(!children.empty()){
        s += "\n";
        s += pad + "<nodes>\n";
        for(auto childP = children.begin(); childP != children.end() ; childP++){
            s += (*childP)->toGexf(indent + 1);
        }
        s += pad + "</nodes>\n";
    }
    s += pad + "</node>\n";
    return s;
}

double Node::getLogLikeContribution(){
    double logLik_cont = 0.0;
    logLik_cont += getLogPrior();
    logLik_cont += getLogLike();

    assert(isfinite(logLik_cont));

    return logLik_cont;
}

/****
 * Statistics
 */
int Node::getDepth(){
    if (isInternalNode()) {
        int max = 0;
        for (auto it = children.begin(); it != children.end(); ++it) {
            int current = (*it)->getDepth();
            if (current > max) {
                max = current;
            }
        }
        return max+1;
    }

    return 1;
}


void Node::getNodeLayerRelation(vector<vector<int> >& layer_matrix,vector<int>& node_order, int current_depth,int min_leaves, int max_leaves){
    
    
    
    
    if (!children.empty()) {
        int i = min_leaves;
        for (auto it = children.begin(); it != children.end(); ++it) {
            int group_id = i+(int)(*it)->getLeavesP()->size()-1;
            int group_min_leaves=i;
            for (auto it_leaf = (*it)->getLeavesP()->begin();
                 it_leaf != (*it)->getLeavesP()->end(); ++it_leaf) {
                
                layer_matrix[current_depth][i] =group_id+1;
                i++;
            }
            
            (*it)->getNodeLayerRelation(layer_matrix, node_order, current_depth+1, group_min_leaves, group_id);
        }
    } else {
        node_order[min_leaves] = nodeId;
        assert(min_leaves == max_leaves);
    }
    
    
    
    
    
};


string Node::toJSON(){
    if (!children.empty()) {
        string s ="{\"name\": \" " + to_string(nodeId) +"\",\"size\": \""+to_string(num_internal_nodes)+"\" ,\n\"children\": [\n ";
        for (auto it = children.begin(); (*it) != children.back(); ++it) {
            s = s+"\t"+(*it)->toJSON()+",\n";
        }
        return s+children.back()->toJSON()+"]\n}";

    } else {
        return "{\"name\": \"" + to_string(nodeId) +"\", \"size\": \""+to_string(1)+"\"}";
    }
}
string Node::toJSON(vector<pair<int,double>> cred){
    double c=0;
    for (auto it = cred.begin(); it != cred.end(); ++it) {
        if (it->first == nodeId) {
            c = it->second;
            break;
        }
    }
    
    if (!children.empty()) {
        string s ="{\"name\": \"" + to_string(nodeId) +"\",\"size\": \""+to_string(num_internal_nodes)+"\" ,";
        
        //Find the crediblilty
        s += "\"credibility\": \""+to_string(c)+"\",\n";
        
        s += "\"children\": [\n ";
        for (auto it = children.begin(); (*it) != children.back(); ++it) {
            s += "\t"+(*it)->toJSON(cred)+",\n";
        }
        return s+children.back()->toJSON(cred)+"]\n}";
        
    } else {
        return "{\"name\": \"" + to_string(nodeId) +"\", \"size\": \""+
                to_string(1)+"\",\"credibility\": \""+to_string(c)+"\"}";
    }
    
    
}


