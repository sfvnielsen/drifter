/*
 * tree.h
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#ifndef TREE_H_
#define TREE_H_

#include <utility>
#include <list>
#include <string>
#include <vector>

#include "node.h"


class Tree {

private:
    Node * rootP;
    std::list<Node> nodes;
    int nextInternalNodeId;

    Adj_list * adjacencyListP;

    int InitFlatTree(int);
    int InitBinaryTree(int);
    Node * makeNsplitTree(int, int, int);

public:
    // Constructors
    Tree();
    Tree(Adj_list * AP);
    Tree(Adj_list * AP, std::string initType);
    Tree(std::list<std::pair<int, int>>, std::string);
    Tree(std::list<std::pair<int, int>>
         , std::vector<int>, Adj_list *);

    //Rule of "5"
    //virtual ~Tree();                           // Default destructor
    Tree(Tree const &);                         // Copy constructor
    Tree& operator=(const Tree& other);         // Copy assignment constructor.
    //Tree(Tree&&) = default;                    // Move constructor
    //Tree& operator=(const Tree&) & = default;  // Copy assignment operator
    //Tree& operator=(Tree&&) & = default;       // Move assignment operator
    bool isEqual(Tree);

    // Get and set - Trivial stuff
    Adj_list * getAdjacencyListP();
    void setRootP(Node *);
    Node * getRoot();
    int getNextInternalNodeId();
    Node * addNode();
    Node * getNode(int);
    void removeNode(Node *);

    // Regrafting
	double regraft();
    double regraft(double, double, int, int);
    void regraft(int,int); // Only used for debug
	Node * getRandomScion();
	Node * getRandomStock();
	Node * cutSubtree(Node * );
    void insertSubtree(Node *, Node *, bool);
    void updateScionAndStock(Node *, Node *, Node *, double, double, int, int);

    // Evaluating likelihood
    double evaluateLogLikeTimesPrior(double, double, int, int);
    bool isLoglikeInitialised = false;

    // Print
    std::string toString();
    void writeMatlabFormat(std::string);
};



#endif /* TREE_H_ */

