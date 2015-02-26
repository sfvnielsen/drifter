/**
 *  Project:        Parmugit
 *  Class:          Node
 *  Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
 *  Affiliation:    Technical University of Denmark
 *  Date:           January 2015
 */

#include <list>
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

   	Node * getParent();
    void setParent(Node *);

    int getNodeId();
    void setNodeId(int);

    std::list<int> * getLeaves();
    int getNumInternalNodes();
    void setNumInternalNodes(int);

    // Children methods
    std::list<Node *> getChildren();
    void setChildren(std::list<Node *>);
    void addChild(Node *);
    bool removeChild(Node *);

    // Node random sampling
    Node * getRandomDescendant();
    int updateNumInternalNodes();

    void updateLeaves();

    // evaluating likelihood
    double evaluateNodeLogLike();
    double evaluateSubtreeLogLike();
    double evaluateNodeLogPrior();
    double evaluatePairLogLike(Node *, Node *);
    std::list<std::pair<int,int>> getCountsAll();
    std::pair<int,int> getCountsPair(Node *, Node *);

    // getting and updating cached likelihood contributions
    double getNodeLogLike();
    void updateNodeLogPrior();
    //void updatePairLogLike();
    void updateAllPairsLogLike();
    void updateChildPairsLogLike(Node *);

    // equality
    bool isEqualSubtree(Node *);
    bool operator == ( const Node &rhs ) const;
    bool isInternalNode();

    // print
    std::string toString();

    void updateScion2Root(Node *, bool);
    void updateStock2Root(Node *, bool);

    double getLogLikeContribution();
    void setLogLikeContribution(double);

private:
    Node * parentP;
	int num_internal_nodes; //Is need for stock sampling
    std::list<Node *> children;
    int nodeId;

    double loglikelihood_cont;
    double log_prior;
    std::vector<double> loglikePair_cont;

    Tree * treeP;
    std::list<int> leaves;
};

double logbeta(double,double);
double lgamma_ratio(double, double);
double log_diff(double, double);

Node * multinomialSampling(std::list<Node *>, std::list<double>);

#endif /* NODE_H_ */
