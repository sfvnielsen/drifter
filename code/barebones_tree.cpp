/*
 * tree.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#include "barebones_tree.h"
#include "adjacency_list.h"

#include <iostream>
#include <cmath>
using namespace std;

/**
 * Construct flat tree
 */
Tree::Tree(list<tuple<int,int>> input_graph) {
	root = Node();
    graph = input_graph; //New name and is it neccesary

    // insert all the indexes from the edge list into leaves
	for (list<tuple<int,int>>::iterator it = graph.begin(); it != graph.end(); it++){
        leaves.push_back(get<0>(*it));
        leaves.push_back(get<1>(*it));
    }

    // Find only the unique elements
    leaves.sort();
    leaves.unique();

    /*
     * Initialisation step, here init is worse case (IRM model),
     * another appoarch is a binary tree (TBI)
     */
    // Add a new Node for each leaf and add is as a child of root
    for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
        nodes.push_back(Node(*it));
        root.addChild(&(nodes.back()));
    }

    int N = leaves.size();
    Adj_list A = Adj_list(N,graph);

}

//Add constructer Tree(input_graph, arbitrary tree structure)
Tree::Tree(list<tuple<int,int>> data_graph, list<tuple<int,int>> tree_struct_graph) {
    //Construct tree from tree_struct_graph
    //Construct adj list from data_graph

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
    double root_node_contribution = this->root.evaluateNodeLogLike(alpha,beta,rho_plus,rho_minus);
    double root_subtree_contribution = this->root.evaluateSubtreeLogLike(alpha
                                                    ,beta,rho_plus,rho_minus);

    return root_node_contribution + root_subtree_contribution;
}


