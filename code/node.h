/*
 * node.h
 *
 *  Created on: Jan 9, 2015
 *      Author: Julian
 */

#include <list>
#include <cmath>

#include "adjacency_list.h"

#ifndef NODE_H_
#define NODE_H_

class Node {
public:
    Node();
	Node(Adj_list *); // default constructor
    Node(Adj_list *, int); // leaf node constructor

    bool isDescendant(Node *);
    void setParent(Node *);
    std::list<std::pair<int,int>> getCountsAll();
    std::pair<int,int> getCountsPair(Node *, Node *);
    std::list<int> getLeaves();
    std::list<Node *> getChildren();
    void addChild(Node *);
    void removeChild(Node *);
    bool isInternalNode();

	Node * getParent();
	int getNumInternalNodes();
	int getNumEdges();
	int getNumPossibleEdges();

    std::string toString();

    double evaluateNodeLogLike(double,double,int,int);
    double evaluateSubtreeLogLike(double,double,int,int);

private:
    Node * parent;
	int num_internal_nodes;
    std::list<Node *> children;
    int leafId;
    bool isInternal;
    Adj_list * AP;


// TODO pointer to adjacency list of the tree

};

double logbeta(double,double);
double lgamma_ratio(double, double);
double log_diff(double a, double b);

#endif /* NODE_H_ */
