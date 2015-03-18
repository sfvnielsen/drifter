/**
 *  Project:        Parmugit
 *  Class:          Node
 *  Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
 *  Affiliation:    Technical University of Denmark
 *  Date:           January 2015
 */

#include <vector>
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

    std::vector<int> * getLeaves();
    void setLeaves(std::vector<int>);
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
    void combineLeavesSet(std::vector<int>,std::vector<int>);
    void addLeaves(std::vector<int>);
    void removeLeaves(std::vector<int>);

    // evaluating likelihood
    double evaluateNodeLogLike(double,double,int,int);
    double evaluateSubtreeLogLike(double,double,int,int);
    std::list<std::pair<int,int>> getCountsAll(); //TO BE DEPRICATED WITH SMART UPDATE
    std::pair<int,int> getCountsPair(Node *, Node *);


    // equality
    bool isEqualSubtree(Node *);
    bool operator == ( const Node &rhs ) const;
    bool isInternalNode();

    // print
    std::string toString();
    std::string toGexf(int);

    void updateScion2Root(Node *, bool);
    void updateStock2Root(Node *, bool);

    double loglikelihood_cont=0;
    double getLogLikeContribution();
    void setLogLikeContribution(double);

private:
    Node * parentP;
    int num_internal_nodes; //Is need for stock sampling
    std::list<Node *> children; //TODO vector
    int nodeId;

    Tree * treeP;
    std::vector<int> leaves; //TODO vector
};

double logbeta(double,double);
double lgamma_ratio(double, double);
double log_diff(double, double);

//template <class someVector, class someElement>;
//bool removeElement(someVector vec, someElement elem);

Node * multinomialSampling(std::list<Node *>, std::vector<double>);  //TODO vector

#endif /* NODE_H_ */
