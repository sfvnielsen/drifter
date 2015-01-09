/*
 * tree.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#include "barebones_tree.h"

#include <iostream>
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
    return Tree();
}




string Tree::toString(){
    return root.toString();
}

/////////////////////////
// Node class functions //
//////////////////////////

Node::Node() {
    // Trivial constructor.
	parent = nullptr;
	num_internal_nodes = 0;
	leaves.clear(); //redundant??
    children.clear(); //redundant??
}

Node::Node(int L) {
    // Construct a node with the leaf L
    // This defines a leaf-node
	parent = nullptr;
    num_internal_nodes = 0;
    leaves.clear(); //redundant??
	leaves.push_back(L);
	children.clear(); //redundant??
}

list<int> Node::getLeaves() {
	return leaves;
}

Node * Node::getParent() {
	return parent;
}

/**
 * Set the pointer "parent" to a new value.
 */
void Node::setParent(Node * new_parent) {
	parent = new_parent;
}

void Node::addChild(Node * childP) {
    // Add a child by:
    //  - Setting the childs parent pointer.
    //  - Adding the childs pointer to the child list.
    //  - Inserting the childs leaves-list on to the end of the parents.
    childP->setParent(this);
    children.push_back(childP);
	leaves.splice(leaves.end(),childP->getLeaves());
}

void removeChild(Node * child){
    cout << "These arent the droids youre looking for" << endl; // TEST
}

bool isInternalNode(Node * n){
    return false;
}


string Node::toString(){
    // Building a string representing the tree by printing all of the leaf-Sets

    string s = "leaves: (";

    if(!leaves.empty()){
        for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
            s += "," + to_string(*it);
        }
    }

    s += ")\n";

    // -- Recurse into children to print the entire subtree.

    s+= "number of children: " + to_string(children.size()) + "\n";

    if(!children.empty()){
        for (list<Node *>::iterator it = children.begin(); it != children.end(); it++){
            Node * childP = *it;
            s += childP->toString();
        }
    }
    return s ;
}



