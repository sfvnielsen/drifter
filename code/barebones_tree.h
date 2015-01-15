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
    /**
     * RULE of 5
     * Destructor
     * copy const
     * copy assignment operator
     * move con
     * move assignment
     *
     *
     *
     */
private:
    Node * rootP;
    std::list<Node> nodes;
    std::list<int> leaves;
    std::vector<int> vec_leaves;
    std::list<std::pair<int, int>> graph;
    Adj_list adjacencyList;
    int InitBinaryTree();
    Node * makeNleafTree(int, int, int);

    int InitFlatTree();
    int nextInternalNodeId;
	// TODO make functions private when not debugging them.

public:
    Tree(); // default constructor
    // Tree(something); // some other constructor?
    Tree(std::list<std::pair<int, int>>); // make the naive tree building in the adjacency matrix.
    Tree(std::list<std::pair<int, int>>, std::string);
    Tree(std::list<std::pair<int, int>>, std::list<std::pair<int, int>>
         , std::vector<int>); // Builds tree based on data, tree and data-tree relation
    Tree(Tree const &);

    void removeNode(Node *);
    Adj_list getAdjacencyList();
    void setRootP(Node *);

	void regraft(); // return new regrafted tree
    void regraft(int,int); // DEBUG testing specific move;
    int getNextInternalNodeId();
	Node * getRandomNode();
	Node * getRoot();
	void cutSubtree(Node * scion_node); //Returns the subtree at Node sub_node
    void insertSubtree(Node * stockP, Node * scionP, bool asChild);
    Node * getNode(int);

    void recalculate(); // calculate all of the counts on the tree from the graph

    std::string toString();

    double evaluateLogLikeTimesPrior(double, double, int, int);

    std::list<std::pair<int, int>> getCountsAll();

};



#endif /* TREE_H_ */

