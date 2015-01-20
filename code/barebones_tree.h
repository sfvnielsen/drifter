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
#include <algorithm>
#include <fstream>
#include "node.h"


class Tree {

private:
    Node * rootP;
    std::list<Node> nodes;
    int nextInternalNodeId;

    Adj_list * adjacencyListP;
 
    int InitFlatTree(int);
    int InitBinaryTree(int);
    Node * makeNleafTree(int, int, int);

public:
    // Constructors
    Tree();
    Tree(Adj_list * AP);
    Tree(Adj_list * AP, std::string initType);
    Tree(std::list<std::pair<int, int>>, std::string);
    Tree(std::list<std::pair<int, int>>
         , std::vector<int>, Adj_list *); // Builds tree based on data, tree and data-tree relation

    //TODO: Rule of "5"
    //virtual ~Tree();                     // Default destructor
    Tree(Tree const &);                  // Copy constructor
    Tree& operator=(const Tree& other); // Copy assignment constructor.
    //Tree(Tree&&) = default;                    // Move constructor
    //Tree& operator=(const Tree&) & = default;  // Copy assignment operator
    //Tree& operator=(Tree&&) & = default;       // Move assignment operator
    bool isEqual(Tree);

    // Get and set - Trivial stuff
    Adj_list * getAdjacencyListP();
    void setRootP(Node *);
    Node * getRoot(); // Returns pointer tree-root
    int getNextInternalNodeId();
    Node * addNode(); // Adds a node to the nodes-list
    Node * getNode(int); // Get node by unique 'leaf-id'
    void removeNode(Node *);  // Removes node from nodes-list (called from Node class)

    // Regrafting
	double regraft(); // return new regrafted tree
    void regraft(int,int); // DEBUG testing specific move;
	Node * getRandomScion();
	Node * getRandomStock();
	int cutSubtree(Node * ); //Modifies the tree and returns change in the number of internal nodes
    int insertSubtree(Node *, Node *, bool); //Modifies the tree and returns change in the number of internal nodes


    // Evaluating likelihood
    double evaluateLogLikeTimesPrior(double, double, int, int);

    // Print
    std::string toString();
    void writeMatlabFormat(std::string);
    
};



#endif /* TREE_H_ */

