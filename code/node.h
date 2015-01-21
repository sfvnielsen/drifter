/*
 * node.h
 *
 *  Created on: Jan 9, 2015 as part of parmugit-project
 *      Author: Jesper, Julian and Søren
 */

#include <list>
#include <cmath>

#include "adjacency_list.h"

#ifndef NODE_H_
#define NODE_H_

class Tree;

class Node {
public:
    Node(); // empty constructor
	Node(Tree *); // default constructor
    Node(Tree * ,int ); // leaf node constructor

    void copyFrom(Tree *, Node const & old_node);

    void setParent(Node *);
   	Node * getParent();

    int getNodeId();
    void setNodeId(int);

    std::list<int> * getLeaves();
    int getNumInternalNodes();

    // Children methods
    std::list<Node *> getChildren();
    void setChildren(std::list<Node *>);
    void addChild(Node *);
    int removeChild(Node *);

    // Node random sampling
    Node * getRandomDescendant();
    int updateNumInternalNodes();
    void updateLeaves();

    // evaluating likelihood
    double evaluateNodeLogLike(double,double,int,int);
    double evaluateSubtreeLogLike(double,double,int,int);
    std::list<std::pair<int,int>> getCountsAll();
    std::pair<int,int> getCountsPair(Node *, Node *);


    // equality
    bool isEqualSubtree(Node *);
    bool operator == ( const Node &rhs ) const;
    bool isInternalNode();

    // print
    std::string toString();

private:
    Node * parentP;
	int num_internal_nodes; //Is need for stock sampling
    std::list<Node *> children;
    int nodeId;

    Tree * treeP;
    std::list<int> leaves;
};

double logbeta(double,double);
double lgamma_ratio(double, double);
double log_diff(double, double);

Node * multinomialSampling(std::list<Node *>, std::list<double>);

#endif /* NODE_H_ */
