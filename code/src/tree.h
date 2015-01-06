/*
 * tree.h
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#ifndef TREE_H_
#define TREE_H_

#include <unordered_map>
#include <vector>

class Tree {
public:
	Tree(); // default constructor
	Tree(something); // some other constructor?

	Node getRandomNode();

	Tree getSubtree(Node sub_root); //Returns the subtree at Node sub_node

private:
};


class LeafSet { // bitstring referring to leaves of tree
public:

private:

};

class Node {
public:
	Node(); // default constructor


private:
	int num_internal_nodes;
	LeafSet leaves;
	std::unordered_map<LeafSet,std::vector<int>> group_parameters;
	Node *parent;
};



#endif /* TREE_H_ */
