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

class Tree;

class Node {
public:
    Node();
	Node(Tree *); // default constructor
    Node(Tree * ,int ); // leaf node constructor

    bool isDescendant(Node *);
    void setParent(Node *);
    void setTreePointer(Tree *);
    int getLeafId();
    void setLeafId(int);
    std::list<std::pair<int,int>> getCountsAll();
    std::pair<int,int> getCountsPair(Node *, Node *);
    std::list<int> getLeaves();
    std::list<Node *> getChildren();
    void setChildren(std::list<Node *>);
    void addChild(Node *);
    void removeChild(Node *);
    bool isInternalNode();
    void setInternalNodeValue(bool);
    bool operator == ( const Node &rhs ) const;

	Node * getParent();
	int getNumInternalNodes();
    int updateNumInternalNodes();
	int getNumEdges();
	int getNumPossibleEdges();

    Node * getRandomDescendant();
    std::string toString();

    double evaluateNodeLogLike(double,double,int,int);
    double evaluateSubtreeLogLike(double,double,int,int);

private:
    Node * parentP;
	int num_internal_nodes;
    std::list<Node *> children;
    int leafId;
    bool isInternal;
    Tree * treeP;

};

double logbeta(double,double);
double lgamma_ratio(double, double);
double log_diff(double a, double b);

Node * multinomialSampling(std::list<Node *>, std::list<double>);

#endif /* NODE_H_ */
