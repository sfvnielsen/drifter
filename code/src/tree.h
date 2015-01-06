/*
 * tree.h
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#ifndef TREE_H_
#define TREE_H_

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
};



#endif /* TREE_H_ */
