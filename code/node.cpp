/*
 * node.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: sfvn
 */

#include "node.h"

#include <iostream> // should be removed at some point
using namespace std;

/////////////////////////
// Node class functions //
//////////////////////////

Node::Node() {
    // Trivial constructor.
    parent = nullptr;
    isInternal = true;
}

Node::Node(int L) {
    // Construct a node with the leaf L
    // This defines a leaf-node
    parent = nullptr;
    num_internal_nodes = 0;
    leafId = L;
    isInternal = false;
}

list<int> Node::getLeaves() {
    list<int> leaves;

    if(!isInternal){
        leaves.push_back(leafId);
        return leaves;
    }

    if(!children.empty()) {
        for (list<Node *>::iterator it = children.begin(); it != children.end(); it++) {
            Node * childP = *it;
            leaves.splice(leaves.end(), childP->getLeaves());
        }
    }

    return leaves;
}

Node * Node::getParent() {
    return parent;
}

/**
 * Set the pointer "parent" to a new value.
 */
void Node::setParent(Node * new_parent) {
    parent = new_parent;
}

void Node::addChild(Node * childP) {
    // Add a child by:
    //  - Setting the childs parent pointer.
    //  - Adding the childs pointer to the child list.
    //  - Inserting the childs leaves-list on to the end of the parents.
    childP->setParent(this);
    children.push_back(childP);
}

void Node::removeChild(Node * child) {
    cout << "These arent the droids youre looking for" << endl; // TEST
}

bool Node::isInternalNode() {
    return isInternal;
}
string Node::toString() {
    // Building a string representing the tree by printing all of the leaf-Sets

    list<int> leaves = this->getLeaves();

    string s = "leaves: (";
    if(!leaves.empty()) {
        for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++) {
            s += "," + to_string(*it);
        }
    }
    s += ")\n";
    // -- Recurse into children to print the entire subtree.
    s += "number of children: " + to_string(children.size()) + "\n";
    if(!children.empty()) {
        for (list<Node *>::iterator it = children.begin(); it != children.end(); it++) {
            Node * childP = *it;
            s += childP->toString();
        }
    }
    return s ;
}


/**
 * Get counts of links and non-links between the pair of children
 */
tuple<int, int> Node::getCountsPair(Node * childAP, Node * childBP) {

    list<int> LA = childAP->getLeaves();
    list<int> LB = childBP->getLeaves();

    // Number of possible links
    int nA = LA.size();
    int nB = LB.size();

    int nPossible = nA*nB;

    int nLinks = 0;

    for (list<int>::iterator fst = LA.begin(); fst != LA.end(); fst++) {
        for (list<int>::iterator snd = LB.begin(); snd != LB.end(); snd++) {
            //result.push_back(this->getCountsPair(*fst, *snd));
        }
    }

    tuple<int, int> result (nLinks,nPossible);
    return result;
}


/**
 * Get counts of links and non-links between all pairs of children
 */
list<tuple<int, int>> Node::getCountsAll() {
    list<tuple<int, int>> result;
    // Loop through each child
    for (list<Node *>::iterator fst = children.begin(); fst != children.end(); fst++) {
        // iterator for the next child
        list<Node *>::iterator nxt = fst;
        // Loop through each child after it in the list
        for (list<Node *>::iterator snd = ++nxt ; snd != children.end(); snd++) {
            result.push_back(this->getCountsPair(*fst, *snd));
        }
    }
    return result;
}


