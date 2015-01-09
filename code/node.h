/*
 * node.h
 *
 *  Created on: Jan 9, 2015
 *      Author: Julian
 */

#include <list>
#include <tuple>

#ifndef NODE_H_
#define NODE_H_

class Node {
public:
	Node(); // default constructor
    Node(int); // leaf node constructor

    bool isDescendant(Node *);
    void setParent(Node *);
    std::list<std::tuple<int,int>> getCountsAll();
    std::tuple<int,int> getCountsPair(Node *, Node *);
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


private:
    Node * parent;
	int num_internal_nodes;
    std::list<Node *> children;
    int leafId;
    bool isInternal;


// TODO pointer to adjacency list of the tree

};

#endif /* NODE_H_ */
