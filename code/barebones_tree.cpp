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

//Add constructer Tree(input_graph, arbitrary tree structure)
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
    Node new_child = Node(& adjacent,element.second);
    new_child.setParent(& root);
    root.addChild(& new_child);
    
    while (!tree_struct_graph.empty()) {
        //Get next relation parrent --> child
        element = tree_struct_graph.front();
        tree_struct_graph.pop_front();
        
        Node * parrent = getNode(element.first);
        
        Node new_child = Node(& adjacent,element.second);
        new_child.setParent(parrent);
        
        parrent->addChild(& new_child);
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
 * Finds a specific node and returns a pointer to this node
 * (Basic implementation!)
 */
Node * Tree::getNode(int id){
    //Iterates over all internal and leaf nodes
    for(list<Node>::iterator it = nodes.begin();
        it != nodes.end(); it++){
        
        if (it->getLeafId() == 1) {
            return &(* it);
        }
        
    }

    
/*    while (first!=last) {
        if (*first==val) return first;
        ++first;
    }
    return last;*/
    
    
    return nullptr;
}

list<pair<int, int>> Tree::getCountsAll(){
    return root.getCountsAll();
}

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


