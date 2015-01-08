/*
 * tree.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#include "barebones_tree.h"

#include <iostream>
using namespace std;


Tree::Tree(list<tuple<int,int>> input_graph) {
	// TODO constructor from graph
	root = Node();

    graph = input_graph;


	for (list<tuple<int,int>>::iterator it = graph.begin(); it != graph.end(); it++){
        //std::cout << '('<<get<0>(*it) << ','<<get<1>(*it)<< ')' << endl;
        leaves.push_back(get<0>(*it));
        leaves.push_back(get<1>(*it));
    }

    leaves.sort();
    leaves.unique();

    for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
        Node aNode = Node(*it);
        nodes.push_back(aNode);
        root.addChild(&aNode);
    }

    //root.toString();

    Node * p = root.children.front();

    cout << p->toString();

    //list<Node *> C = root.children;
    //list<Node *>::iterator it = C.begin();

   // for (list<Node *>::iterator it = C.begin(); it != C.end(); it++){
        //cout << (*it)->toString();
        //Node * childP = *it;
        //Node child = *childP;
//        string s = childP;
        //s += child.toString();
    //}

}


string Tree::toString(){
    return root.toString();
}

/////////////////////////
// Node class functions //
//////////////////////////

Node::Node() {
	parent = nullptr;
}

Node::Node(int L) {
	parent = nullptr;
	leaves.push_front(L);
}

list<int> Node::getLeaves() {
	return leaves;
}

Node * Node::getParent() {
	return parent;
}

void Node::setParent(Node * new_parent) {
	parent = new_parent;
}

void Node::addChild(Node * childP) {
    childP->setParent(this);
	children.push_back(childP);
	leaves.splice(leaves.end(),childP->getLeaves());
}


string Node::toString(){
    string s = "leaves: (";

    if(!leaves.empty()){
        cout << leaves.size() << endl;
        for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
            cout << *it << endl;
            //s += "," + ;
        }
    }

    s += ")\n";

    //for (list<Node *>::iterator it = children.begin(); it != children.end(); it++){
    //    Node * childP = *it;
        //Node child = *childP;
//        string s = childP;
        //s += child.toString();
    //}

    return "Node\n"+s;
}



