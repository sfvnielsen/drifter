/*
 * tree.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#include "barebones_tree.h"


#include <iostream>
#include <cmath>
using namespace std;

/**
 * Construct flat tree
 */
Tree::Tree(list<pair<int,int>> data_graph) {

    // insert all the indexes from the edge list into leaves
	for (list<pair<int,int>>::iterator it = data_graph.begin(); it != data_graph.end(); it++){
        leaves.push_back(it->first);
        leaves.push_back(it->second);
    }

    // Find only the unique elements
    leaves.sort();
    leaves.unique();

    int N = (int) leaves.size();
    adjacencyList = Adj_list(N,data_graph);

    nodes.push_back(Node(this));
    rootP = &(nodes.back());

    /*
     * Initialisation step, here init is worse case (IRM model),
     * another appoarch is a binary tree (TBI)
     */
    // Add a new Node for each leaf and add is as a child of root
    for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
        nodes.push_back(Node(this,*it));
        rootP->addChild(&(nodes.back()));
    }
    rootP->updateNumInternalNodes();
}

/**
* Special test constructor
* (First element in tree_struct_graph must contain root)
*/
Tree::Tree(list<pair<int,int>> data_graph, list<pair<int,int>> tree_struct_graph,
           vector<int> data_leaf_relation) {

    // - Construct adj list from data_graph
    int N = (int) data_leaf_relation.size();
    adjacencyList = Adj_list(N, data_graph);

    // 1.0 Construct the tree from tree_struct_graph
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
//        cout << "Is null?: " << (parent == nullptr) << endl; // DEBUG
//        cout << "Found node: " << parent->getLeafId() << endl; // DEBUG
        new_child = Node(this,element.second);

        Node* existing_nodeP = this->getNode(element.second);
        if (existing_nodeP==nullptr) {
            nodes.push_back(new_child);
            parent->addChild(&(nodes.back()));
        } else{
            parent->addChild(existing_nodeP);
        }
//        cout << "First: " << element.first << " Second: " << element.second << endl; // DEBUG

    }

    //Updates internal count.
    rootP->updateNumInternalNodes();


    /*
     * For each leaf node, correct the leaf ID, so it correspond to the data ID
     *  each internal node is assigned a unique negative number.
     */
    int new_id =-1;
    rootP->setLeafId(new_id);

    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        if (it->isInternalNode()){ //Internal node
            new_id--;
            it->setLeafId(new_id);

        } else { //Leaf node
             //Find what the fake_id corresponds to in real id
            int fake_id = it->getLeafId();
            it->setLeafId(data_leaf_relation[fake_id]);
        }
    }

}
/**
 * Finds a specific node (characterized by an unique id) and
 * returns a pointer to this node or nullptr if it isn't pressent
 * (Basic implementation!)
 */
Node * Tree::getNode(int leaf_id){
    //Iterates over all internal and leaf nodes
    if (leaf_id == 0) { // is root?
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

list<pair<int, int>> Tree::getCountsAll(){
    return rootP->getCountsAll();
}


/**
* Get random node in tree (recursive operation)
* - Chooses internal nodes with weight 2 and leaves with weight 1
* - calls getRandomChild from node-class
*/

Node * Tree::getRandomNode() {
    return rootP->getRandomDescendant();
}

void Tree::removeNode(Node * nodeP){
    nodes.remove(*nodeP);
}

Adj_list Tree::getAdjacencyList(){
    return adjacencyList;
}

void Tree::setRootP(Node * node){
    rootP = node;
}




/**
 *
 */
Tree Tree::regraft(){
    Tree new_Tree = *this;
// TODO: finish the regrafting
    Node * scionP = new_Tree.getRandomNode();
    if(!(scionP==rootP)){
        new_Tree.cutSubtree(scionP);
        rootP->updateNumInternalNodes();
        Node * stockP = new_Tree.getRandomNode();
        // TODO: random child or sibling
        new_Tree.insertSubtree(stockP, scionP, true);
        rootP->updateNumInternalNodes();
    }
    return new_Tree;
}

string Tree::toString(){
    string sAdj = adjacencyList.toString();
    return rootP->toString() + sAdj ;
}


/**
* Evaluating log-likelihood x prior of tree
*/

double Tree::evaluateLogLikeTimesPrior(double alpha, double beta, int rho_plus, int rho_minus){
//    double root_node_contribution = this->rootP->evaluateNodeLogLike(alpha,beta,rho_plus,rho_minus);
//    double root_subtree_contribution = this->rootP->evaluateSubtreeLogLike(alpha
//                                                    ,beta,rho_plus,rho_minus);
//
//    return root_node_contribution + root_subtree_contribution;
    return rootP->evaluateSubtreeLogLike(alpha,beta,rho_plus,rho_minus);
}

void Tree::cutSubtree(Node * scionP){
    cout << "cutting: " << scionP->getLeafId() << endl;
    // assumes that scionP doesnt point to root.
    Node * parentP = scionP->getParent();
    parentP->removeChild(scionP);
    scionP->setParent(nullptr);

    // TODO: remove obsoleted internal nodes
}

Node * Tree::getRoot(){
    return rootP;
}

void Tree::insertSubtree(Node * stockP, Node * scionP, bool asChild){
    if(asChild){
        stockP->addChild(scionP);
    }else{

        // TODO: Replacing the root node.

        // cut the stock
        Node * parent = stockP->getParent();
        parent -> removeChild(stockP);

        // add a new parent
        nodes.push_back(Node());
        Node * new_parent = &(nodes.back());
        parent->addChild(new_parent);

        // add graft both stock and scion to the new parent
        new_parent->addChild(stockP);
        new_parent->addChild(scionP);
    }
}
