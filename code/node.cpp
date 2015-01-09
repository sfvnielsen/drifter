/*
 * node.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: sfvn
 */

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
	leaves.splice(leaves.end(),childP->getLeaves());
}

void removeChild(Node * child){
    cout << "These arent the droids youre looking for" << endl; // TEST
}

bool isInternalNode(Node * n){
    return false;
}

string Node::toString(){


/**
 * Get counts of links and non-links between the pair of children
 */
tuple<int,int> Node::getCountsPair(Node * childAP, Node * childBP){
}


/**
 * Get counts of links and non-links between all pairs of children
 */
list<tuple<int,int>> getCountsAll(){
    // Loop through each child
    for (list<Node *>::iterator fst = children.begin(); fst != children.end(); fst++){
        // Loop through each child after it in the list
        for (list<Node *>::iterator snd = fst ; snd != children.end(); snd++){

        cout << fst->toString() ;
        }
    }
}




    // Building a string representing the tree by printing all of the leaf-Sets

    string s = "leaves: (";

    if(!leaves.empty()){
        for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
            s += "," + to_string(*it);
        }
    }

    s += ")\n";

    // -- Recurse into children to print the entire subtree.

    s+= "number of children: " + to_string(children.size()) + "\n";

    if(!children.empty()){
        for (list<Node *>::iterator it = children.begin(); it != children.end(); it++){
            Node * childP = *it;
            s += childP->toString();
        }
    }
    return s ;
}
