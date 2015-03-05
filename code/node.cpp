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
#include <numeric>  // lets us use cumulative sum
#include <cassert> // for assert statements
#include <random> // C++11 random generator
#include <cmath> //lgamma and other math functions
#include <stdexcept>
#include <vector>

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
vector<int> * Node::getLeaves() {
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

bool Node::removeChildCached(Node * childP) {

    int N = (int) children.size()-1; //A node is removed
    vector<double> new_pairLogLike(N*(N-1)/2,0);
    
    auto it = pairLogLikeCont.begin();
    auto it_new = new_pairLogLike.begin();
    
    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        auto nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            if( !(*fst == childP || *snd == childP) ){
                (*it_new) = (*it);
                ++it_new;
            }
            ++it;
        }
    }
    
    pairLogLikeCont = new_pairLogLike;
    children.remove(childP);
    
    return false;
}

void Node::addChildCached(Node * childP) {
    
    children.push_back(childP); //A node is added
    int N = (int) children.size();
    vector<double> new_pairLogLike(N*(N-1)/2,0);
    
    auto it = pairLogLikeCont.begin();
    auto it_new = new_pairLogLike.begin();
    
    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        auto nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            if(*snd == childP){ // der skal tilføjes noget
                (*it_new) = evaluatePairLogLike(*fst, childP);
            } else {
                (*it_new) = (*it);
                ++it;
            }
            ++it_new;
            
        }
    }
    pairLogLikeCont = new_pairLogLike;
    
}

//Kald på parent af nodeToReplace
void Node::replaceChild(Node * nodeToReplace, Node * new_node){
    //Erstat
    if (this != nullptr) {
        auto it = pairLogLikeCont.begin();
        //Updatere loglike contribution
        for (auto fst = children.begin(); fst != children.end(); fst++) {
            // iterator for the next child
            auto nxt = fst;
            // Loop through each child after it in the list
            for (auto snd = ++nxt ; snd != children.end(); snd++) {
                if(*snd == nodeToReplace){ // der skal tilføjes noget
                    (*it) = evaluatePairLogLike(*fst, new_node);
                } else if (*fst == nodeToReplace) {
                    (*it) = evaluatePairLogLike(*snd, new_node);
                } else {
                    //No change in parameters/loglike
                }
                ++it;
                
            }
        }
        //Erstat nodeToReplace med new_node i children listen
        for (auto it = children.begin(); it!=children.end(); ++it) {
            if (*it == nodeToReplace) {
                (*it) = new_node;
                break;
            }
        }
    } else {
        //???
    }
    
    
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

    //Otherwise it is an internal node
    list<Node *> node_list = getChildren();
    vector<int> subtree_weight;

    // Each child is a subtree and it weight is calculated and stored for each subtree (child)
    for (auto it = node_list.begin(); it!= node_list.end(); ++it ) {
        subtree_weight.push_back(2*((*it)->getNumInternalNodes())+
                                 (int)(((*it)->getLeaves())->size()) );
        //Checks for consistency in isInternal
        assert((*it)->getNumInternalNodes() == 0 || (*it)->isInternalNode());
        assert((*it)->getNumInternalNodes()> 0 || !(*it)->isInternalNode());

    }
    //The weights summes to the weight of the subtree rooted at the current node
    int sum_weight = (int)((getLeaves())->size())+
                            2*getNumInternalNodes();
    //The weights are normalised to a probability and store in vector p_vals
    vector<double> p_vals;
    for (auto it = subtree_weight.begin(); it!= subtree_weight.end(); ++it) {
        p_vals.push_back((double)*it/sum_weight);
    }

    p_vals.push_back((double)2/sum_weight); // Root probability added
    node_list.push_back(this);

    // assert that p_vals sums to 1
    double p_val_sum = 0;
    p_val_sum = accumulate(p_vals.begin(),p_vals.end(),p_val_sum);
    assert(abs(p_val_sum-1.0)<1e-12);

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
    assert(abs(cumulative_sum.back()-1.0 < 1e-12));
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
            vector<int> childLeaves = *(childP->getLeaves());
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
        loglikelihood_cont = 0.0;
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
    //Caches loglikelihood_cont, which is the non-normalized posterior
    loglikelihood_cont = log_like+log_prior;
    return log_like+log_prior;
};

double Node::evaluateLogPrior(){
    
    double alpha = treeP->alpha;
    double beta = treeP->beta;

    // Prior contribution for each node
    double log_prior = 0.0;
    
    int num_children = (int) (getChildren()).size();
    int num_leaves_total = (int) (getLeaves())->size();
    vector<int> num_leaves_each_child;
    list<Node *> list_of_children = getChildren();
    
    // Get number of leaves for each child
    for (auto it = list_of_children.begin();
         it!= list_of_children.end(); ++it) {
        int num_leaves = (int) (*it)->getLeaves()->size();
        num_leaves_each_child.push_back(num_leaves);
    }
    
    // Special case if alpha is zero
    if (alpha == 0.0){
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

    return log_prior;
}

double Node::evaluatePairLogLike(Node * childAP, Node * childBP){
    int num_links, num_pos_links;
    double log_like;
    
    int rho_plus = treeP->rho_plus;
    int rho_minus = treeP->rho_minus;
    
    pair<int, int> counts = getCountsPair(childAP,childBP);
    
    num_links = counts.first;
    num_pos_links = counts.second;
    log_like =  logbeta(num_links+rho_plus,
                        num_pos_links-num_links+rho_minus)
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
 * Get counts of links and non-links between all pairs of children
 */
list<pair<int, int>> Node::getCountsAll() {
    list<pair<int, int>> result;
    // Loop through each child
    for (auto fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        list<Node *>::iterator nxt = fst;
        // Loop through each child after it in the list
        for (auto snd = ++nxt ; snd != children.end(); snd++) {
            result.push_back(getCountsPair(*fst, *snd));
        }
    }
    return result;
}

/**
 * Get counts of links and non-links between the pair of children
 */
pair<int, int> Node::getCountsPair(Node * childAP, Node * childBP) {

    vector<int> * LA = childAP->getLeaves();
    vector<int> * LB = childBP->getLeaves();

    // Number of possible links
    int nA = (int) LA->size();
    int nB = (int) LB->size();

    int nPossible = nA*nB;

    int nLinks = 0;

    Adj_list * adjacency_list = treeP->getAdjacencyListP();

    // Loop through all all combinations of leaves and check if they are connected
    for (auto fst = LA->begin(); fst != LA->end(); fst++) {
        for (auto snd = LB->begin(); snd != LB->end(); snd++) {
            if(adjacency_list->isConnected(*fst,*snd)){
                nLinks += 1;
            }
        }
    }

    pair<int, int> result (nLinks,nPossible);
    return result;
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
    
    return log_like;
}

/**
 * Get cached log prior
 */
double Node::getLogPrior(){
    return logPrior;
}

/**
 * Update cached node prior
 */
void Node::updateNodeLogPrior(){
    logPrior = evaluateLogPrior();
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

    vector<int> leavesOriginal  = *getLeaves(),
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
 * Checks whether a node an internal node, by looking at its children or lack of
 */
bool Node::isInternalNode() {
    return !children.empty();
}

/**
 * Printing by recursing through the subtree root at this node
 */
string Node::toString() {
    // Building a string representing the tree by printing all of the leaf-Sets
    vector<int> leaves = *(getLeaves());
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

double Node::getLogLikeContribution(){
    return loglikelihood_cont;
}
void Node::setLogLikeContribution(double new_contribution){
    loglikelihood_cont = new_contribution;
}
