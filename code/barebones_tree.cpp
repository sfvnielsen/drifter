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
Tree::Tree(list<tuple<int,int>> data_graph) {

    // insert all the indexes from the edge list into leaves
	for (list<tuple<int,int>>::iterator it = data_graph.begin(); it != data_graph.end(); it++){
        leaves.push_back(get<0>(*it));
        leaves.push_back(get<1>(*it));
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
    // 1.0: Construct tree from tree_struct_graph
/*    tuple<int,int> element = tree_struct_graph.front();
    tree_struct_graph.pop_front(); //Remove element
    root = get<0>(element);
    root.addChild(element.get(1))
    
    while (!tree_struct_graph.empty()) {
        element = tree_struct_graph.pop_front();
        //Find element.get(0) // parrent
        //Insert element.get(1) as child
    }
    */
    // 1.1:
        //Update leaf id
        //update internal id and status
    
    
    // - Construct adj list from data_graph
//    N =
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

list<tuple<int, int>> Tree::getCountsAll(){
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


