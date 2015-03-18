/**
 *  Project:        Parmugit
 *  Class:          Tree
 *  Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
 *  Affiliation:    Technical University of Denmark
 *  Date:           January 2015
 */

#include "tree.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cassert>
//#include <numeric>
#include <random>

using namespace std;

/**
 * Construct flat tree from number of leaves
 * - i.e. Root has only leaves as children
 */
Tree::Tree(Adj_list * AP): nextInternalNodeId(0) {
    isLoglikeInitialised = false;
    adjacencyListP = AP;
    InitFlatTree(AP->getSize() );
    // recurses through tree and updates cached values
    rootP->updateNumInternalNodes();
    rootP->updateLeaves();
}

/**
 * Tree constructor choice
 * - choose between flat and binary tree
 */
Tree::Tree(Adj_list * AP, string initType): nextInternalNodeId(0) {
    isLoglikeInitialised = false;
    adjacencyListP = AP;
    if (initType == "Binary") {
        InitBinaryTree(AP->getSize());
    } else { //Flat tree
        InitFlatTree(AP->getSize());
    }
    //Assure correct node fields by recursion through tree
    rootP->updateNumInternalNodes();
    rootP->updateLeaves();

}

/**
* Special test constructor
* (First element in tree_struct_graph must contain root)
* NB!: Only used to pass likelihood test in debugging phase
*/
Tree::Tree(list<pair<int,int>> tree_struct_graph,
           vector<int> data_leaf_relation, Adj_list * adj_list): nextInternalNodeId(0) {
    isLoglikeInitialised = false;
    adjacencyListP = adj_list;

    // - Construct the tree from tree_struct_graph
    //Get first relation parrent --> child, assumption the root is first
    pair<int,int> element = tree_struct_graph.front();
    tree_struct_graph.pop_front();

    //Construct the parent
    nodes.push_back(Node(this, element.first));
    rootP = &(nodes.back());

    //Add the second node as a child
    Node new_child = Node(this, element.second);

    nodes.push_back(new_child);
    rootP->addChild(&(nodes.back()));

    //Insert parrent-->child relations for the rest
    while (!tree_struct_graph.empty()) {
        //Get next relation parrent --> child
        element = tree_struct_graph.front();
        tree_struct_graph.pop_front();

        Node * parent = this->getNode(element.first);

        //Check if the child node is already created,
        //  then it is a nullptr if it does not exist
        Node* existing_nodeP = this->getNode(element.second);
        if (existing_nodeP==nullptr) {
            new_child = Node(this,element.second);
            nodes.push_back(new_child);
            parent->addChild(& (nodes.back()));
        } else{ //Exists, and we just need to add a pointer to its child
            parent->addChild(existing_nodeP);
        }


    }
    //Updates internal count.
    rootP->updateNumInternalNodes();

    /*
     * For each leaf node, correct the leaf ID, so it correspond to the data ID
     *  each internal node is assigned a unique negative number.
     */
    rootP->setNodeId(getNextInternalNodeId());
    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        if (it->isInternalNode()){ //Internal node
            it->setNodeId(getNextInternalNodeId());

        } else { //Leaf node
             //Find what the fake_id corresponds to in real id
            int fake_id = it->getNodeId();
            it->setNodeId(data_leaf_relation[fake_id]);
        }
    }
    rootP->updateLeaves();
}

/**
 * Initializes the data in a binary tree structure
 */
int Tree::InitBinaryTree(int num_leaves){
    setRootP(makeNsplitTree(0, num_leaves -1 ,2) );
    return 0;
}

/**
 * Makes N-split Tree s.t. each split is into N nodes (if possible)
 */
Node * Tree::makeNsplitTree(int a, int b, int N){

    //If (b-a) < N, we are a leaf level in the tree (or just above).
    if ((b-a) < N) {
        //Special case: There is only ONE node (a leaf), so the leaf should
        //  be added as a child to the previous internal node.
        if (b==a){
            //Creating leaf node
            nodes.push_back(Node(this,a));
            Node * child_P = & nodes.back();
            return child_P;
        //General case: There is between 2 and N nodes (leafs), they are added
        //  to a new internal node, which is a child to the previous internal node
        } else {
            //Create new internal node
            nodes.push_back(Node(this,getNextInternalNodeId()));
            Node * parent = & nodes.back();

            //Add the leaf nodes as children
            for (int i = 0; (i < N) && (i <= (b-a)) ; i++) {
                nodes.push_back(Node(this,a+i));
                Node * child_P = & nodes.back();
                parent->addChild(child_P);
            }
            return parent;
        }
    //else, We are at a level where we should split, by adding an internal node
    //       and recursing into N new (internal) nodes.
    } else {
        //Create internal node, which
        nodes.push_back(Node(this,getNextInternalNodeId()));
        Node * parent = & nodes.back();

        Node * new_child;// = makeNleafTree(a, (b-a)/N+a, N);;
        for (int i = 0; i < N; i++) {
            if (i == N-1){
                new_child = makeNsplitTree(i*((b-a+1)/N)+a, b, N);
            } else {
                new_child = makeNsplitTree(i*((b-a+1)/N)+a, (i+1)*((b-a+1)/N)+a-1, N);
            }

            parent->addChild(new_child);
        }
        return parent;
    }
}

/**
 * Initialise a flat treee structure
 *  NB! Initialisation step, here init is worse case for parameters (flat tree)
 *  - Add a new node for each leaf and add it as a child of root
 */
int Tree::InitFlatTree(int num_leaves){
    // add new nodes to tree
    nodes.push_back(Node(this,getNextInternalNodeId()));
    rootP = &(nodes.back());

    // add nodes as children of root
    for (int i = 0; i < num_leaves; i++){
        nodes.push_back(Node(this,i));
        rootP->addChild(&(nodes.back()));

    }

    rootP->updateNumInternalNodes();
    rootP->updateLeaves();
    return 0;
}

/**
 * Copy constructor
 */
Tree::Tree(Tree const &old_tree){
    adjacencyListP = old_tree.adjacencyListP;
    nextInternalNodeId = old_tree.nextInternalNodeId;
    isLoglikeInitialised = old_tree.isLoglikeInitialised;

    nodes.push_back(Node(this,getNextInternalNodeId()));
    rootP = &(nodes.back());

    // Copy all root information
    rootP->copyFrom(this, *(old_tree.rootP));
}

/**
* Copy assignment constructor
*/
Tree& Tree::operator=(const Tree& other) {
    // check for self-assignment
    if(&other == this)
        return *this;

    // else copy fields
    adjacencyListP = other.adjacencyListP;
    nextInternalNodeId = other.nextInternalNodeId;
    isLoglikeInitialised = other.isLoglikeInitialised;

    nodes.clear();
    nodes.push_back(Node(this,getNextInternalNodeId()));
    rootP = &(nodes.back());

    rootP->copyFrom(this, *(other.rootP));
    return *this;
}

/**
 TODO: Rest of "Rule of 5"
 - Destructor
 - Move
 - Move-assign
 */

/**
 * Tests if two trees are equal, defined as when all internal nodes in the
 *  original tree have the same children as the internal nodes in the "copy" tree.
 *
 * Also symmetric tree structure is allowed, as long as each node fulfills the
 *  same leaves requirement.
 */
bool Tree::isEqual(Tree copy_tree){
    return rootP->isEqualSubtree(copy_tree.rootP);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Get and Set - "Trivial" Stuff the proof is up to the reader
 */


 /** Get pointer to adjacency lit */
Adj_list * Tree::getAdjacencyListP(){
    return adjacencyListP;
}

/** Get pointer to root node */
Node * Tree::getRoot(){
    return rootP;
}

/** Set root Node */
void Tree::setRootP(Node * node){
    rootP = node;
}

/** Decrements nextInternalNodeId and returns it */
int Tree::getNextInternalNodeId(){
    return --nextInternalNodeId;
}

/** Adds a new Node to the nodes list */
Node * Tree::addNode(){
    nodes.push_back(Node(this,getNextInternalNodeId()));
    return &(nodes.back());
}

/**
 * Finds a specific node (characterized by an unique id) and
 * returns a pointer to this node or nullptr if it isn't present
 * (Basic implementation!)
 */
Node * Tree::getNode(int node_id){
    //Iterates over all internal and leaf nodes
    if (node_id == rootP->getNodeId()) { // is root?
        return rootP;
    }
    // else loop trough nodes list
    for(auto it = nodes.begin();
        it != nodes.end(); it++){

        if (it->getNodeId() == node_id) {
            return &(* it);
        }

    }
    // if not found return null
    return nullptr;
}

/** Removes specific node from nodes list
 * - Used when collapsing branch in regrafting
 */
void Tree::removeNode(Node * nodeP){
    nodes.remove(*nodeP);
}

/** Regrafting */

/**
 * Modifies tree by random regrafting (Naive implementation)
 * Returns the Metropolis-Hasting 'move-ratio' (NB! non-logarithmic)
 * - Steps are
 *      - Sample scion (node to be removed)
 *      - Cut respective subtree
 *      - Update relevant attributes on tree (naive)
 *      - Sample stock (insertion position)
 *      - Insert scion at stock (either as child or as sibling)
 *      - Update relevant attributes on tree (naive)
 *      - Return ratio of move probabilities
 */
double Tree::regraft(){
    // Sample removal node
    Node * scionP = this->getRandomScion();
    if(!(scionP==rootP)){
        // Probability of choosing scion
        int n_nodes = (int)nodes.size();
        double p_scion = 1.0/(n_nodes);

        // Cut and update
        cutSubtree(scionP);
        rootP->updateNumInternalNodes();
        rootP->updateLeaves();

        // Sample insertion
        Node * stockP = this->getRandomStock();

        // Coinflip (random C++11 standard)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution dis(0.5);
        bool unbiased_coinflip = dis(gen);

        // Insert subtree as child or sibling and update tree
        insertSubtree(stockP, scionP, unbiased_coinflip);
        rootP->updateNumInternalNodes();
        rootP->updateLeaves();

        // Probability of choosing stock node
        n_nodes = (int) nodes.size();
        double p_stock = 1.0/n_nodes;

        return p_stock/p_scion;
    } else{ // scion was root - ratio of move probabilities is 1
        return 1;
    }
}

/**
 * Modifies tree by random regrafting (smart update)
 * Updates the likelihood at each node on the fly
 * Returns the Metropolis-Hasting 'move-ratio' (NB! non-logarithmic)
 * - Steps are
 *      - Sample scion (node to be removed)
 *      - Cut respective subtree
 *          - While cutting: Update relevant attributes on tree
 *      - Sample stock (insertion position)
 *      - Insert scion at stock (either as child or as sibling)
 *           - While inserting: Update relevant attributes
 *       - Update log-likelihood contribution on two branches:
 *           - Scion -> Root
 *           - Stock -> Root
 *      - Return ratio of move probabilities
 */
double Tree::regraft(double alpha, double beta, int rho_plus, int rho_minus){
    // Get random scion
    Node * scionP = this->getRandomScion();
    if(!(scionP==rootP)){
        // Probability of choosing scion
        int n_nodes = (int)nodes.size();
        double p_scion = 1.0/(n_nodes);

        // Cut subtree rooted at scion (and update accordingly)
        Node *  scionParentP = cutSubtree(scionP);

        // Sample stock (insertion)
        Node * stockP = this->getRandomStock();

        // Coinflip object (C++11 standard)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution dis(0.5);
        bool unbiased_coinflip = dis(gen);

        // Insert cut subtree as child or sibling to stock
        // Update accordingly (inside insertSubtree)
        insertSubtree(stockP, scionP, unbiased_coinflip);

        // Update node-loglike on paths to root
        updateScionAndStock(scionP, scionParentP, stockP,
                            alpha,beta,rho_plus,rho_minus);

        // Move probabilities
        n_nodes = (int) nodes.size();
        double p_stock = 1.0/n_nodes;
        return p_stock/p_scion;
    } else{ // scion was root - ratio of move probabilities is 1
        return 1;
    }
}


/**
 * Regraft specific nodes !!! ASSUMES ITS A VALID OPERATION!!
 * NB! Only used for debugging purposes
 */
void Tree::regraft(int scionVal, int stockVal){
    // TODO: finish the regrafting
    Node * scionP = this->getNode(scionVal);
    if(!(scionP==rootP)){
        cout << "cutting: " << scionP->getNodeId() << endl;
        this->cutSubtree(scionP);
        rootP->updateNumInternalNodes();
        rootP->updateLeaves();

        Node * stockP = this->getNode(stockVal);
        cout << "inserting: " << stockP->getNodeId() << endl;
        // TODO: random child or sibling
        this->insertSubtree(stockP, scionP, false);
        rootP->updateNumInternalNodes();
        rootP->updateLeaves();
    }
}

/**
 *    Get random node in tree (based on nodes list)
 *    All nodes are weigthed equally
 *    Used for sampling the SCION (node to remove)
 */
Node * Tree::getRandomScion() {
    int num_nodes = (int) nodes.size();
    // C++11 random generator objects
    std::random_device rd;
    std::mt19937 random_generator(rd());
    std::uniform_int_distribution<> dis(0,num_nodes-1);
    // Sample random id in nodes list
    int random_node_id = dis(random_generator);
    list<Node>::iterator it = nodes.begin();
    for (int i = 0; i!= random_node_id; ++i) {
            // loop through list until you find the element
        ++it;
    }
    return &(*it);
}

/**
* Get random node in tree (recursive operation)
* - Chooses internal nodes with weight 2 and leaves with weight 1
* - calls getRandomChild from node-class
* - Used for sampling the STOCK!
*/
Node * Tree::getRandomStock() {
    return rootP->getRandomDescendant();
}

/**
* Cuts subtree rooted at scion i.e.
*    - cuts connection between subtree and rest of tree
*     - but doesnt remove them from nodes list
* Updates leaves and number of internal nodes where it is necessary
* - Returns the nodeP from where the following likelihood update should start from
*/
Node * Tree::cutSubtree(Node * scionP){
    // Remove scion from parents child list (and collapse if needed)
    Node * parentP = scionP->getParent();
    Node * grandParentP = parentP->getParent();
    bool collapsed = parentP->removeChild(scionP);

    // Set parent to null
    scionP->setParent(nullptr);
    // Save what leaves and number of internal nodes scion had
    vector<int> leaves_to_rem = *(scionP->getLeaves() );
    int internal_nodes_rem = scionP->getNumInternalNodes()+ (int) collapsed;

    Node * currentP;
    Node * parent_to_return;

    if (collapsed) {
        if(grandParentP==nullptr){ //Parent is root
            //in this case the last remaining node is root and nothing should be done
            return rootP;
        }else{
            //Scions parent were collapsed,
            // so the update should start from its grand parent
            parent_to_return = grandParentP;
            currentP = grandParentP;
        }
    } else { //Scion has atleast 2 siblings, so removing it does not collaps any nodes
        parent_to_return = parentP;
        currentP = parentP;
    }
    //Remove leafs and internal node count up to and including root
    while (currentP != nullptr) {
        currentP->setNumInternalNodes(currentP->getNumInternalNodes()-internal_nodes_rem);
        assert(currentP->getNumInternalNodes()>=0);
        currentP->removeLeaves(leaves_to_rem);
        currentP = currentP->getParent();
    }
    return parent_to_return;
}

/**
* Inserts subtree (cut from above method) and modifies tree accordingly
*  - inserts either as child or as sibling of stock-node
*  - Updates leaves and number of internal nodes based on inserted subtree
*/
void Tree::insertSubtree(Node * stockP, Node * scionP, bool asChild){

    vector<int> leaves_to_add;
    // Cannot be added as a child to a leaf, only as sibling
    if (! stockP->isInternalNode()){
        asChild = false;
    }
    //Update from stock to root, add leaves and num. internal nodes
    if(asChild){
        stockP->addChild(scionP);
    }else{ //As sibling
        // Create a new node
        Node * new_parent = addNode();
        // Constuct and add a new parent
        Node * stock_parent = stockP->getParent();
        if(stock_parent != nullptr){ // if stock is not root
            new_parent->setParent(stock_parent);
            stock_parent->addChild(new_parent);
            stock_parent->removeChild(stockP);
        } else {// if stock is root
            setRootP(new_parent);
        }

        // Add both scion and stock as children
        new_parent->addChild(stockP);
        new_parent->addChild(scionP);

        //Update the num. internal and leaves
        new_parent->setNumInternalNodes(stockP->getNumInternalNodes());
        leaves_to_add = *(stockP->getLeaves() );
        new_parent->addLeaves(leaves_to_add);

    }

    // Start from current node and update up to root
    leaves_to_add = *(scionP->getLeaves() );
    int internal_nodes_add = scionP->getNumInternalNodes()+ (int) !asChild;

    Node * currentP = scionP->getParent();
    while (currentP != nullptr) {
        leaves_to_add = *(scionP->getLeaves() );
        currentP->setNumInternalNodes(currentP->getNumInternalNodes()+internal_nodes_add);
        currentP->addLeaves(leaves_to_add);

        currentP = currentP->getParent();
    }
}

/**
 * Updates log-like contribution of each node along two paths
 * - From Scion -> Root
 * - From Stock -> Root
 */


void Tree::updateScionAndStock(Node * scionP, Node * oldScionParentP, Node* stockP
                               ,double alpha, double beta, int rho_plus, int rho_minus){

    // Relation between scion and stock after insert
    assert(scionP->getParent()==stockP || scionP->getParent()==stockP->getParent());
    Node * scionPathP = scionP->getParent();
    Node * stockPathP = oldScionParentP;

    // Update scion path
    while (scionPathP != nullptr) {
        scionPathP->evaluateNodeLogLike(alpha, beta,rho_plus, rho_minus);
        scionPathP = scionPathP->getParent();
    }

    // Update stock path
    while (stockPathP != nullptr) {
        stockPathP->evaluateNodeLogLike(alpha, beta,rho_plus, rho_minus);
        stockPathP = stockPathP->getParent();
    }
}


///////////////////////////////////////////////////////////////////////////////
/**
* Evaluating log(likelihood x prior) of tree
* Either:
*   - Recurses through tree and calculates every nodes contribution
*   - Or , sums up each nodes current set contribution
*/
double Tree::evaluateLogLikeTimesPrior(double alpha, double beta, int rho_plus, int rho_minus){
    if (isLoglikeInitialised) {
        double sum = 0.0;
        for (auto it = nodes.begin(); it != nodes.end(); ++it) {
            sum += it->getLogLikeContribution();
        }
        return sum;
    } else { // if log likelihood hasnt been calculated recurse through
        isLoglikeInitialised = true;
        return rootP->evaluateSubtreeLogLike(alpha,beta,rho_plus,rho_minus);
    }
}


/** Printing */
string Tree::toString(){
    cout << "I am root: " << rootP->getNodeId() << "; num_int: "<< rootP->getNumInternalNodes() << endl;
    return rootP->toString();
}


/** Write out the tree as a .txt file formatted
    for ease of use in MATLAB scripts (plotting)
*/
void Tree::writeMatlabFormat(string filename) {

    // Give all nodes a proper new id
    list<Node *> lnodes; // recursion list
    lnodes.push_back(rootP);

    int current_id = 0;
    vector<int> new_id(nodes.size(),-1);
    while (!lnodes.empty()) {
        Node * cnodeP = lnodes.front(); // take next node
        list<Node>::iterator it_nodes = nodes.begin();
        int n_id = 0;
        while (&(*it_nodes)!= cnodeP) { // find its id in nodes list
            it_nodes++;
            n_id++;
        }
        new_id[n_id] = current_id++; // set its new id
        list<Node*> c_children = cnodeP->getChildren(); // add children to recursion-list
        for (auto it_c = c_children.begin(); it_c!= c_children.end(); ++it_c) {
            lnodes.push_back(*it_c);
        }
        lnodes.pop_front(); // remove current node
    }

    // Parent -> Child relation list
    int n_this_node = 0;
    vector<int> parent_child_rel((int)nodes.size(),-1);
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        Node * cparentP = it->getParent(); // get pointer to this nodes parent
        if (cparentP != nullptr) {
            // find location of parent in nodes list
            list<Node>::iterator it_parent = nodes.begin();
            int n_id_parent = 0;
            while (&(*it_parent) != cparentP) {
                it_parent++;
                n_id_parent++;
            }
            // At position = this_nodes_id, set parents new_id
            // NB! +1 due to MATLAB not zero-indexing
            parent_child_rel[new_id[n_this_node]] = new_id[n_id_parent]+1;
        } else { // if root
            parent_child_rel[0] = 0;
        }
        n_this_node++;
    }

    // Leaf->Data list
    vector<int> leaf_list(adjacencyListP->getSize(),-1);
    for (int i = 0; i < adjacencyListP->getSize(); i++){
        // find corresponding node in nodes
        list<Node>::iterator it_node = nodes.begin();
        int node_id = 0;
        while (i != it_node->getNodeId()) {
            it_node++;
            node_id++;
        }
        leaf_list[i] = new_id[node_id]+1;
    }

    assert(find(leaf_list.begin(),leaf_list.end(),-1) == leaf_list.end());

    ofstream out_file(filename.c_str()); // output file

    // write first line - Parent->Child relation
    for (auto it = parent_child_rel.begin(); it != parent_child_rel.end(); ++it) {
        out_file << *it << " ";
    }
    out_file << "\n";
    // write second line - Leaf-Data relation
    for (auto it = leaf_list.begin(); it != leaf_list.end(); ++it) {
        out_file << *it << " ";
    }
}

/** Write out the tree as a gexf file
    for ease use with Gephi
*/
void Tree::writeGexf(string filename) {

    // Give all nodes a proper new id
    list<Node *> lnodes; // recursion list
    lnodes.push_back(rootP);

    int current_id = 0;
    vector<int> new_id(nodes.size(),-1);
    while (!lnodes.empty()) {
        Node * cnodeP = lnodes.front(); // take next node
        auto it_nodes = nodes.begin();
        int n_id = 0;

        while (&(*it_nodes)!= cnodeP) { // find its id in nodes list
            it_nodes++;
            n_id++;
        }
        new_id[n_id] = current_id++; // set its new id

        list<Node*> c_children = cnodeP->getChildren(); // add children to recursion-list
        for (auto it_c = c_children.begin(); it_c!= c_children.end(); ++it_c) {
            lnodes.push_back(*it_c);
        }
        lnodes.pop_front(); // remove current node
    }

    int n_id = 0;
    for(auto it = nodes.begin(); it != nodes.end(); ++it){
        (*it).setNodeId(new_id[n_id]);
        n_id++;
    }

    ofstream out_file(filename.c_str()); // output file

}


string Tree::toGexf(){
    string s =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<gexf xmlns=\"http://www.gexf.net/1.2draft\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.gexf.net/1.2draft http://www.gexf.net/1.2draft/gexf.xsd\" version=\"1.2\">"
                "<graph mode=\"static\" defaultedgetype=\"undirected\">\n";
    s += rootP->toGexf(0); // 0 as the starting indent;
    s += adjacencyListP->toGexf();
    s += "</graph>\n";
    s += "</gexf>";
    return s;
}


