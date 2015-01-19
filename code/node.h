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

    void copyFrom(Tree *, Node const & old_node);

    bool isDescendant(Node *);
    void setParent(Node *);
   	Node * getParent();

    void setTreePointer(Tree *);

    int getLeafId();
    void setLeafId(int);

    void updateLeaves();
    std::list<int> * getLeaves();
    std::list<std::pair<int,int>> getCountsAll();
    std::pair<int,int> getCountsPair(Node *, Node *);

    std::list<Node *> getChildren();
    void setChildren(std::list<Node *>);
    void addChild(Node *);
    int removeChild(Node *);

    bool isInternalNode();
    void setInternalNodeValue(bool);


	int getNumInternalNodes();
    int updateNumInternalNodes();

    int getNumEdges();
	int getNumPossibleEdges();
    Node * getRandomDescendant();


    double evaluateNodeLogLike(double,double,int,int);
    double evaluateSubtreeLogLike(double,double,int,int);
    
    bool isEqual(Node *);
    bool operator == ( const Node &rhs ) const;
    std::string toString();

private:
    Node * parentP;
	int num_internal_nodes;
    std::list<Node *> children;
    int leafId;
    bool isInternal;
    Tree * treeP;
    std::list<int> leaves;
    bool modified;
};

double logbeta(double,double);
double lgamma_ratio(double, double);
double log_diff(double a, double b);

Node * multinomialSampling(std::list<Node *>, std::list<double>);

#endif /* NODE_H_ */
