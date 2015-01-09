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
#include "node.h"


class Tree {
private:
    Node root;
    std::list<Node> nodes;
    std::list<int> leaves;
    std::list<std::tuple<int,int>> graph;
    Adj_list A;

	// TODO make functions private when not debugging them.

public:
	Tree(); // default constructor
	// Tree(something); // some other constructor?
	Tree(std::list<std::tuple<int,int>>); // make the naive tree building in the adjacency matrix.
    Tree(std::list<std::tuple<int,int>> data_graph, std::list<std::tuple<int,int>> tree_struct_graph);
	Tree regraft(); // return new regrafted tree
	Node * getRandomNode();
	Tree cutSubtree(Node * sub_root); //Returns the subtree at Node sub_node

	void recalculate(); // calculate all of the counts on the tree from the graph

    std::string toString();

	double evaluateLikelihood();

    std::list<std::tuple<int,int>> getCountsAll();

};


double logbeta(double,double);


#endif /* TREE_H_ */

