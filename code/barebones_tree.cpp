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
	root = Node();
    graph = input_graph;

    // insert all the indexes from the edge list into leaves
	for (list<tuple<int,int>>::iterator it = graph.begin(); it != graph.end(); it++){
        leaves.push_back(get<0>(*it));
        leaves.push_back(get<1>(*it));
    }

    // Find only the unique elements
    leaves.sort();
    leaves.unique();

    /*
     * Initialisation step, here init is worse case (IRM model),
     * another appoarch is a binary tree (TBI)
     */
    // Add a new Node for each leaf and add is as a child of root
    for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
        nodes.push_back(Node(*it));
        root.addChild(&(*nodes.end()));
    }

    cout << "--- After constructing the tree (in the constructor) ---" << endl;

    cout << root.toString();

    Node * p = root.children.front();
    Node c = *p;

    cout << "Size of child.leaves: " << p->getLeaves().size() << endl;


    list<int> L = p->getLeaves();


    cout << "Size of copy of child.leaves: " << L.size() << endl;
        for (list<int>::iterator it = L.begin(); it != L.end(); it++){
            cout << "," << to_string(*it) ;
        }


    cout << "the child.toString:" <<p->toString();

    // gives segfault when acessing the childs leaves list

}


string Tree::toString(){
    return root.toString();
}

/////////////////////////
// Node class functions //
//////////////////////////

Node::Node() {
    // Trivial constructor.
	parent = nullptr;
	num_internal_nodes = 0;
}

Node::Node(int L) {
    // Construct a node with the leaf L
    // This defines a leaf-node
	parent = nullptr;
    num_internal_nodes = 0;
	leaves.push_back(L);
}

list<int> Node::getLeaves() {
	return leaves;
}

Node * Node::getParent() {
	return parent;
}

void Node::setParent(Node * new_parent) {
    // Set the pointer "parent" to a new value.
	parent = new_parent;
}

void Node::addChild(Node * childP) {
    // Add a child by:
    //  - Setting the childs parent pointer.
    //  - Adding the childs pointer to the child list.
    //  - Inserting the childs leaves-list on to the end of the parents.
    childP->setParent(this);
	children.push_back(childP);
	leaves.splice(leaves.end(),childP->getLeaves());
}


string Node::toString(){
    // Building a string representing the tree by printing all of the leaf-Sets

    string s = "leaves: (";

    if(!leaves.empty()){
        cout << "Size of leaves:" <<leaves.size() << endl;
        for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
            s += "," + to_string(*it);
        }
    }

    s += ")\n";

    // -- Recurse into children to print the entire subtree.

    if(!leaves.empty()){
        for (list<Node *>::iterator it = children.begin(); it != children.end(); it++){
            Node * childP = *it;
            s += childP->toString();
        }
    }
    return s ;
}



