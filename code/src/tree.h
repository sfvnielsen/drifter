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
#include <bitset>
#include <boost/dynamic_bitset.hpp>

class Tree {
public:
	Tree(); // default constructor
	Tree(something); // some other constructor?

	Tree regraft(); // return new regrafted tree
	Node * getRandomNode();
	Tree cutSubtree(Node sub_root); //Returns the subtree at Node sub_node



	double evaluateLikelihood();
private:
};


class Node {
public:
	Node(); // default constructor
	Node(something); // other than default constructor

	void setLeaves(boost::dynamic_bitset<>);
	void setParent(Node *);
	void setNumInternalNodes(int);
	void groupAddEdge(boost::dynamic_bitset<>, std::tuple<int,int>);
	void groupRemoveEdge(boost::dynamic_bitset<>, std::tuple<int,int>);

	boost::dynamic_bitset<> getLeaves();
	Node * getParent();
	int getNumInternalNodes();
	int getNumEdges();
	int getNumPossibleEdges();
	std::list<std::tuple<int,int>> getGroupEdgeList(boost::dynamic_bitset<>);


private:
	int num_internal_nodes;
	boost::dynamic_bitset<> leaves;
	Node *parent;
	std::unordered_map<boost::dynamic_bitset<>, std::list<std::tuple<int, int>>> group_edges; // least common anscestor
};

#endif /* TREE_H_ */

