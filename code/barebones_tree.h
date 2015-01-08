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
#include <string>

	// TODO neccesary libs?

class Node {
public:
	Node(); // default constructor
    Node(int); // leaf node constructor

    bool isDescendant(Node *);
    void setParent(Node *);
    std::tuple<int,int> getCounts(Node *, Node *);
    std::list<int> getLeaves();
    std::list<Node *> getChildren();
    void addChild(Node *);

	Node * getParent();
	int getNumInternalNodes();
	int getNumEdges();
	int getNumPossibleEdges();

    std::string toString();



private:
    Node * parent;
    std::list<Node *> children;
	int num_internal_nodes;
    std::list<int> leaves;

};

class Tree {
private:
    Node * root;
    std::list<Node> nodes;
    std::list<int> leaves;
    std::list<std::tuple<int,int>> graph;

	// TODO make functions private when not debugging them.

public:
	Tree(); // default constructor
	// Tree(something); // some other constructor?
	Tree(std::list<std::tuple<int,int>>); // make the naive tree building in the adjacency matrix.

	Tree regraft(); // return new regrafted tree
	Node * getRandomNode();
	Tree cutSubtree(Node * sub_root); //Returns the subtree at Node sub_node

	void recalculate(); // calculate all of the counts on the tree from the graph

    std::string toString();

	double evaluateLikelihood();

};



#endif /* TREE_H_ */

