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
	Node root = Node();

    graph = input_graph;

    set<int> leafSet;

	for (list<tuple<int,int>>::iterator it = graph.begin(); it != graph.end(); it++){
        std::cout << '('<<get<0>(*it) << ','<<get<1>(*it)<< ')' << endl;
        leafSet.insert(get<0>(*it));
        leafSet.insert(get<1>(*it));
    }

    for (set<int>::iterator it = leafSet.begin(); it != leafSet.end(); it++){
        leaves.push_back(*it);
        cout << *it << endl;
    }


    for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
        //Node aNode = Node(*it);
        //nodes.push_front(aNode);
        //root.addChild(&aNode);
    }

}


string Tree::toString(){
    return root->toString();
}

/////////////////////////
// Node class functions //
//////////////////////////

Node::Node() {
	parent = nullptr;
}

Node::Node(int L) {
	parent = nullptr;
	leaves.push_back(L);
}

void Node::setParent(Node *new_parent) {
	*parent = *new_parent;
}

void Node::addChild(Node * child) {
	children.push_back(child);
}

list<int> Node::getLeaves() {
	return leaves;
}

Node * Node::getParent() {
	return parent;
}


string Node::toString(){
    string s = "leaves: ";

    list<int> L = {2,3,1};


    std::cout << leaves.empty() << endl;
    list<int>::iterator it = leaves.begin();
    //std::cout << *it << endl;


    // TODO SEGFAULT when acessing leaves ?!?!?!?!?!

    for (list<int>::iterator it = L.begin(); it != L.end(); it++){
        s += ',' + to_string(*it);
    }

    //for (list<Node *>::iterator it = children.begin(); it != children.end(); it++){
            //Node * childP = *it;

            //s += childP->toString();
    //}

    return "Node\n"+s;
}



