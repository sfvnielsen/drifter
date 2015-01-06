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
#include <tuple>
#include <list>

class Tree {
public:
	Tree(); // default constructor
	Tree (something); // some other constructor?

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
	Node (something); // other than default constructor

	void setLeaves(LeafSet);
	void setParent(Node *);
	void setNumInternalNodes(int);

	LeafSet getLeaves();
	Node * getParent();
	int getNumInternalNodes();

private:
	int num_internal_nodes;
	LeafSet leaves;
	Node *parent;
	std::unordered_map<LeafSet, std::list<std::tuple<int, int>>> group_parameters; // least common anscestor
};

#endif /* TREE_H_ */

