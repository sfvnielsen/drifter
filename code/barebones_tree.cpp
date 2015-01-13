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

    int N = leaves.size();
    A = Adj_list(N,data_graph);

    root = Node(&A);

    /*
     * Initialisation step, here init is worse case (IRM model),
     * another appoarch is a binary tree (TBI)
     */
    // Add a new Node for each leaf and add is as a child of root
    for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
        nodes.push_back(Node(&A,*it));
        root.addChild(&(nodes.back()));
    }
}

/**
* Special test constructor
* (First element in tree_struct_graph must contain root)
*/
Tree::Tree(list<pair<int,int>> data_graph, list<pair<int,int>> tree_struct_graph,
           list<pair<int,int>> data_leaf_relation) {

    // - Construct adj list from data_graph
    int N = (int) data_leaf_relation.size();
    Adj_list adjacent = Adj_list(N, data_graph);

    // 1.0 Construct the tree from tree_struct_graph
    //Get first relation parrent --> child, assumption the root is first
    pair<int,int> element = tree_struct_graph.front();
    tree_struct_graph.pop_front();

    root = Node(& adjacent,element.first); //set root node
    //Add the first node as a child
    Node new_child = Node(&adjacent,element.second);
    //new_child.setParent(&root);

    nodes.push_back(new_child);
    root.addChild(&(nodes.back()));

    while (!tree_struct_graph.empty()) {
        //Get next relation parrent --> child
        element = tree_struct_graph.front();
        tree_struct_graph.pop_front();

        Node * parent = this->getNode(element.first);
        cout << "Is null?: " << (parent == nullptr) << endl; // DEBUG
        cout << "Found node: " << parent->getLeafId() << endl; // DEBUG
        new_child = Node(& adjacent,element.second);
        //new_child.setParent(parent);
        Node* existing_nodeP = this->getNode(element.second);
        if (existing_nodeP==nullptr) {
            nodes.push_back(new_child);
            parent->addChild(& (nodes.back()));
        } else{
            parent->addChild(existing_nodeP);
        }
        cout << "First: " << element.first << " Second: " << element.second << endl; // DEBUG

    }
    // 1.1:
        //Update leaf info
        //Update internal and count?



    // - Leaves should know what node in the data_graph

    // Tree structure -
    // TODO: - Also load in relation between data graph and tree_struct_graph???
    //
    //for (list<tuple<int,int>>::iterator it = tree_struct_graph.begin();
    //        it!= tree_struct_graph.end(); ++it ) {
      //  if
    //}

    // insert all the indexes from the edge list into leaves
	// - Loop over "relation-list" ??



    // Adjacency list
    //int N = ??
    //A = Adj_list(N,data_graph);
}
/**
 * Finds a specific node (characterized by an id) and
 * returns a pointer to this node
 * (Basic implementation!)
 */
Node * Tree::getNode(int leaf_id){
    //Iterates over all internal and leaf nodes
    if (leaf_id == 0) { // is root?
        return &(this->root);
    }
    cout << "getNode::: Size of nodes: "<<nodes.size() << endl;
    for(list<Node>::iterator it = nodes.begin();
        it != nodes.end(); it++){

        if (it->getLeafId() == leaf_id) {
            return &(* it);
        }

    }


/*    while (first!=last) {
        if (*first==val) return first;
        ++first;
    }
    return last;*/

    cout << "getNode: Found nothing" << endl; // DEBUG!!!
    return nullptr;
}

list<pair<int, int>> Tree::getCountsAll(){
    return root.getCountsAll();
}


/**
* Get random node in tree (recursive operation)
* - Chooses internal nodes with weight 2 and leaves with weight 1
* - calls getRandomChild from node-class
*/

//Node * Tree::getRandomNode() {
//    return root->getRandomDescendant();
//}

/**
 *
 */
Tree Tree::regraft(){
    return *this;
}

string Tree::toString(){
    return root.toString();
}


/**
* Evaluating log-likelihood x prior of tree
*/

double Tree::evaluateLogLikeTimesPrior(double alpha, double beta, int rho_plus, int rho_minus){
//    double root_node_contribution = this->root.evaluateNodeLogLike(alpha,beta,rho_plus,rho_minus);
//    double root_subtree_contribution = this->root.evaluateSubtreeLogLike(alpha
//                                                    ,beta,rho_plus,rho_minus);
//
//    return root_node_contribution + root_subtree_contribution;
    return this->root.evaluateSubtreeLogLike(alpha,beta,rho_plus,rho_minus);
}

Node * Tree::getRandomNode(){
    return root.getChildren().back();
}

void Tree::cutSubtree(Node * scionP){
    Node * parentP = scionP->getParent();
    parentP->removeChild(scionP);
    scionP->setParent(nullptr);
    // TODO: remove obsoleted internal nodes
}

Node * Tree::getRoot(){
    return &root;
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
