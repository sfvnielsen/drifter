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

    void setParent(Node *);
   	Node * getParent();

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


	int getNumInternalNodes();
    int updateNumInternalNodes();

    Node * getRandomDescendant();


    double evaluateNodeLogLike(double,double,int,int);
    double evaluateSubtreeLogLike(double,double,int,int);

    bool isEqualSubtree(Node *);
    bool operator == ( const Node &rhs ) const;
    std::string toString();

    void updateScion2Root(Node *, Node *, bool);
    void updateStock2Root(Node *, Node *, bool);
    void updateScionAndStock(Node *, Node*, Node*, double, double, int, int);
    double loglikelihood_cont=0;
    double getLogLikeContribution();
    void setLogLikeContribution(double);
    void setNumInternalNodes(int);
    bool isNCA(Node *);
private:
    Node * parentP;
	int num_internal_nodes; //Is need for random selection
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
