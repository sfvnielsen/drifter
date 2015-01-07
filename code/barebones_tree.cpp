/*
 * tree.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#include "barebones_tree.h"

Tree::Tree() {
	// TODO Auto-generated constructor stub

}

Tree::Tree(std::list<std::tuple<int,int>> graph) {
	// TODO constructor from graph
	Node root = Node();

	for(int i=graph.begin(); i != graph.end(); ++i){
		leafSet.
	}
}

Tree::~Tree() {
	// TODO Auto-generated destructor stub
}



//////////////////////////
// Node class functions //
//////////////////////////

Node::Node(): num_internal_nodes(0) {
	// First boring default constructor sets every member to respective default values

	// NB! When initializing dynamic_bitset - an allocator object should be used
	// How should unordered_map be initialized? And do we need to supply special hash-function?
	*parent = NULL;

}



void Node::setLeaves(boost::dynamic_bitset<> new_leaves) {
	leaves = new_leaves;
}

void Node::setParent(Node *new_parent) {
	*parent = *new_parent;
}

void Node::setNumInternalNodes(int new_num_internal_nodes) {
	num_internal_nodes = new_num_internal_nodes;
}


boost::dynamic_bitset<> Node::getLeaves() {
	return leaves;
}

Node * Node::getParent() {
	return *parent;
}
