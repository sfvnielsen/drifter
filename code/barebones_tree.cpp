/*
 * tree.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#include "barebones_tree.h"

#include <iostream>
#include <cmath>
#include <cassert>
using namespace std;


/**
 * Construct flat tree from data
 */
Tree::Tree(list<pair<int,int>> data_graph): nextInternalNodeId(0) {

    // insert all the indexes from the edge list into leaves
    for (list<pair<int,int>>::iterator it = data_graph.begin(); it != data_graph.end(); it++){
        leaves.push_back(it->first);
        leaves.push_back(it->second);
    }

    // Find only the unique elements
    leaves.sort();
    leaves.unique();

    //Copies the elements to a vector ***** TEMPORARY *****
    vec_leaves = vector<int>();
    copy(leaves.begin(), leaves.end(), back_inserter(vec_leaves));


    int N = (int) leaves.size();
    adjacencyList = Adj_list(N,data_graph);

    InitFlatTree();
    rootP->updateNumInternalNodes();
}

/**
 * Tree constructor choice
 */
Tree::Tree(list<pair<int,int>> data_graph, string initType): nextInternalNodeId(0) {

    // insert all the indexes from the edge list into leaves
    for (list<pair<int,int>>::iterator it = data_graph.begin(); it != data_graph.end(); it++){
        leaves.push_back(it->first);
        leaves.push_back(it->second);
    }

    // Find only the unique elements
    leaves.sort();
    leaves.unique();

    //Copies the elements to a vector ***** TEMPORARY *****
    vec_leaves = vector<int>();
    copy(leaves.begin(), leaves.end(), back_inserter(vec_leaves));
    cout << "Vec_leaves size: " << vec_leaves.size() << endl;
    cout << "Leaves size: " << leaves.size() << endl << flush;


    int N = (int) leaves.size();
    adjacencyList = Adj_list(N,data_graph);


    if (initType == "Binary") {
        InitBinaryTree();
    } else { //Flat tree
        InitFlatTree();
    }

    //Correct internal number count
    rootP->updateNumInternalNodes();

}

/**
* Special test constructor
* (First element in tree_struct_graph must contain root)
*/
Tree::Tree(list<pair<int,int>> data_graph, list<pair<int,int>> tree_struct_graph,
           vector<int> data_leaf_relation): nextInternalNodeId(0) {

    // - Construct adj list from data_graph
    int N = ((int) data_leaf_relation.size())/2; //Number of leaves in graph
    adjacencyList = Adj_list(N, data_graph);

    // - Construct the tree from tree_struct_graph
    //Get first relation parrent --> child, assumption the root is first
    pair<int,int> element = tree_struct_graph.front();
    tree_struct_graph.pop_front();

    nodes.push_back(Node(this, element.first));
    rootP = &(nodes.back());

    //Add the first node as a child
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
    rootP->setLeafId(getNextInternalNodeId());

    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        if (it->isInternalNode()){ //Internal node
            it->setLeafId(getNextInternalNodeId());

        } else { //Leaf node
             //Find what the fake_id corresponds to in real id
            int fake_id = it->getLeafId();
            it->setLeafId(data_leaf_relation[fake_id]);
            leaves.push_back(it->getLeafId());
        }
    }

}

/**
 * Initialises the data in a binary tree structure
 */
int Tree::InitBinaryTree(){
    setRootP(makeNleafTree(0, (int) vec_leaves.size() -1 ,2) );
    return 0;
}

/**
 *
 */
Node * Tree::makeNleafTree(int a, int b, int N){

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
            parent->setInternalNodeValue(true);

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
        parent->setInternalNodeValue(true);

        //Binary split
//        Node * new_child = makeNleafTree(a, (b-a)/2+a, N);
//        parent->addChild(new_child);
//
//        new_child = makeNleafTree((b-a)/2+a+1, b, N);
//        parent->addChild(new_child);

        Node * new_child;// = makeNleafTree(a, (b-a)/N+a, N);;
        for (int i = 0; i < N; i++) {
            if (i == N-1){
                new_child = makeNleafTree(i*((b-a+1)/N)+a, b, N);
                cout << "parm1: "<< i*((b-a+1)/N)+a << "  ,  parmb: " << b <<endl;
            } else {
                new_child = makeNleafTree(i*((b-a+1)/N)+a, (i+1)*((b-a+1)/N)+a-1, N);
                cout << "parm1: "<< i*((b-a+1)/N)+a << "  ,  parm2: " << (i+1)*((b-a+1)/N)+a-1 <<endl;
            }

            parent->addChild(new_child);
        }
        return parent;
    }


    //return nullptr; //Return nullptr when root is selected.
}

/**
 * Initialise a flat treee structure
 */
int Tree::InitFlatTree(){
    /*
     * Initialisation step, here init is worse case for parameters (flat tree).
     */
    // Add a new Node for each leaf and add is as a child of root

    nodes.push_back(Node(this,getNextInternalNodeId()));
    rootP = &(nodes.back());


    for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
        nodes.push_back(Node(this,*it));
        rootP->addChild(&(nodes.back()));
    }
    rootP->updateNumInternalNodes();
    return 0;
}




/**
* Copy constructor
*
*/
Tree::Tree(Tree const &old_tree){
    leaves = old_tree.leaves;
    graph = old_tree.graph;
    adjacencyList = old_tree.adjacencyList;
    nextInternalNodeId = old_tree.nextInternalNodeId;

    nodes.push_back(Node(this,getNextInternalNodeId()));
    rootP = &(nodes.back());

    rootP->copyFrom(this, *(old_tree.rootP));
}



/**
 TODO: Rest of "Rule of 5"
 - Move
 - Copy-assign
 - Move-assign
 */

///////////////////////////////////////////////////////////////////////////////

/**
 * Get and Set - "Trivial" Stuff the proof is up to the reader
 */

/** Get Data Adjacency List */
Adj_list Tree::getAdjacencyList(){
    return adjacencyList;
}

Adj_list * Tree::getAdjacencyListP(){
    return &adjacencyList;
}

/** Set Root Node */
void Tree::setRootP(Node * node){
    rootP = node;
}

/** Get pointer to root node */
Node * Tree::getRoot(){
    return rootP;
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
 * returns a pointer to this node or nullptr if it isn't pressent
 * (Basic implementation!)
 */
Node * Tree::getNode(int leaf_id){
    //Iterates over all internal and leaf nodes
    if (leaf_id == rootP->getLeafId()) { // is root?
        return rootP;
    }
    for(list<Node>::iterator it = nodes.begin();
        it != nodes.end(); it++){

        if (it->getLeafId() == leaf_id) {
            return &(* it);
        }

    }
    return nullptr;
}

/** Removes specific node from nodes list
 * - Used when collapsing branch in regrafting
 */
void Tree::removeNode(Node * nodeP){
    nodes.remove(*nodeP);
}

///////////////////////////////////////////////////////////////////////////////


/** Regrafting */


/**
 * Modifies tree by random regrafting
 * Returns the Metropolis-Hasting 'move-ratio' (NB! non-logarithmic)
 */
double Tree::regraft(){
// TODO: finish the regrafting
    Node * scionP = this->getRandomScion();
    if(!(scionP==rootP)){
//        cout << "cutting: " << scionP->getLeafId() << endl;
        int n_collapsed = this->cutSubtree(scionP);
        rootP->updateNumInternalNodes();

//        cout << "Mutilated tree :'( \n" << toString() << endl << flush;

        Node * stockP = this->getRandomStock();
//        cout << "inserting at: " << stockP->getLeafId();
        // TODO: random child or sibling
        bool unbiased_coinflip = ((double) rand()/RAND_MAX) > 0.5;
//        cout << " , as child : "+to_string(unbiased_coinflip) << endl;
        int n_created = this->insertSubtree(stockP, scionP, unbiased_coinflip);
        rootP->updateNumInternalNodes();

        // Move probabilities
        int n_nodes = (int)nodes.size();
        double p_scion = 1.0/(n_nodes);
        double p_stock = 1.0/(n_nodes - n_collapsed + n_created);

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
        cout << "cutting: " << scionP->getLeafId() << endl;
        this->cutSubtree(scionP);
        rootP->updateNumInternalNodes();

        Node * stockP = this->getNode(stockVal);
        cout << "inserting: " << stockP->getLeafId() << endl;
        // TODO: random child or sibling
        this->insertSubtree(stockP, scionP, false);
        rootP->updateNumInternalNodes();
    }
}


/**
    Get random node in tree (based on nodes list)
    All nodes are weigthed equally
    Used for sampling the SCION
*/
Node * Tree::getRandomScion() {
    int num_nodes = (int) nodes.size();
    // Sample random id in nodes list
    int random_node_id = (int) round( ((double)rand()/RAND_MAX)*(num_nodes-1));
    list<Node>::iterator it = nodes.begin();
    for (int i = 0; i!= random_node_id; ++i) {
            // loop through list until you find the element
            // NB! Convert all this to vector for random access PLZ!!!?!?
        ++it;
    }
    return &(*it);
}



/**
* Get random node in tree (recursive operation)
* - Chooses internal nodes with weight 2 and leaves with weight 1
* - calls getRandomChild from node-class
* - Usd for sampling the STOCK!
*/
Node * Tree::getRandomStock() {
    return rootP->getRandomDescendant();
}


/**
* Cuts subtree rooted at scion
* , i.e. cuts connection between subtree and rest of tree
* but doesnt remove them from nodes list
* - Returns the number of nodes that have been removed from the tree
*/
int Tree::cutSubtree(Node * scionP){
    // assumes that scionP doesn't point to root.
    Node * parentP = scionP->getParent();
    int collapsed = parentP->removeChild(scionP);
    scionP->setParent(nullptr);
    return collapsed;
}

/**
* Inserts subtree (cut from above method) and modifies tree accordingly
*  - inserts either as child or as sibling of stock-node
*  - returns number of nodes created by insertion (0 or 1)
*/
int Tree::insertSubtree(Node * stockP, Node * scionP, bool asChild){

    int created = 0;
    //TODO: Fix less hot hotfix
    // Cannot be added as a child to a leaf, only as sibling
    if (! stockP->isInternalNode()){
        asChild = false;
    }

    if(asChild){
        stockP->addChild(scionP);
    }else{ //As sibling

        // Create a new node
        Node * new_parent = addNode();
        new_parent->setInternalNodeValue(true);

        // Constuct and add a new parent
        Node * stock_parent = stockP->getParent();
        if(stock_parent != nullptr){ // if stock is not root
            new_parent->setParent(stock_parent);
            stock_parent->addChild(new_parent);
            stock_parent->removeChild(stockP);
        } else {// if stock is root
            setRootP(new_parent);
        }


        created++;

        new_parent->addChild(stockP);
        new_parent->addChild(scionP);
    }
    return created;
}

///////////////////////////////////////////////////////////////////////////////

/**
* Evaluating log-likelihood x prior of tree
* - Recurses through tree
*/

double Tree::evaluateLogLikeTimesPrior(double alpha, double beta, int rho_plus, int rho_minus){
    return rootP->evaluateSubtreeLogLike(alpha,beta,rho_plus,rho_minus);
}


/** Printing */

string Tree::toString(){
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
    vector<int> leaf_list((int)leaves.size(),-1);
    for (auto it = leaves.begin(); it != leaves.end(); ++it) {
        // find corresponding node in nodes
        list<Node>::iterator it_node = nodes.begin();
        int node_id = 0;
        while (*it != it_node->getLeafId()) {
            it_node++;
            node_id++;
        }
        // set leaf_list[*it] = new_id[node_id];
        leaf_list[*it] = new_id[node_id]+1;
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
