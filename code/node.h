/*
 * node.h
 *
 *  Created on: Jan 9, 2015
 *      Author: Julian
 */

#ifndef NODE_H_
#define NODE_H_

class Node {
public:
	Node(); // default constructor
    Node(int); // leaf node constructor

    bool isDescendant(Node *);
    void setParent(Node *);
    std::tuple<int,int> getCounts(Node *, Node *);
    std::list<int> getLeaves();
    std::list<Node *> getChildren();
    void addChild(Node *);
    void removeChild(Node *);
    bool isInternalNode(Node *);

	Node * getParent();
	int getNumInternalNodes();
	int getNumEdges();
	int getNumPossibleEdges();

    std::string toString();
    std::list<int> leaves;


private:
    Node * parent;
	int num_internal_nodes;
    std::list<Node *> children;


};

#endif /* NODE_H_ */
