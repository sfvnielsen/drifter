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
#include <utility>
#include <set>
#include <list>
	// TODO neccesary libs?

class Node {
public:
	Node(); // default constructor
	// Node(something); // other than default constructor

	void setParent(Node *);
	void setNumInternalNodes(int);

	Node * getParent();
	int getNumInternalNodes();
	int getNumEdges();
	int getNumPossibleEdges();


private:
	int num_internal_nodes;
};

class Tree {
private:

	// TODO make functions private when not debugging them.

public:
	Tree(); // default constructor
	// Tree(something); // some other constructor?
	Tree(std::list<std::pair<int,int>>); // make the naive tree building in the adjacency matrix.

	Tree regraft(); // return new regrafted tree
	Node * getRandomNode();
	Tree cutSubtree(Node * sub_root); //Returns the subtree at Node sub_node

	void recalculate(); // calculate all of the counts on the tree from the graph

	std::set<int> leafSet;



	double evaluateLikelihood();

};



#endif /* TREE_H_ */

