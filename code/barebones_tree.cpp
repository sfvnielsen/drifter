/*
 * tree.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#include "barebones_tree.h"


#include <iostream>
#include <cmath>
using namespace std;

/**
 * Construct flat tree
 */
Tree::Tree(list<pair<int,int>> data_graph) {

    // insert all the indexes from the edge list into leaves
	for (list<pair<int,int>>::iterator it = data_graph.begin(); it != data_graph.end(); it++){
        leaves.push_back(it->first);
        leaves.push_back(it->second);
    }

    // Find only the unique elements
    leaves.sort();
    leaves.unique();

    int N = (int) leaves.size();
    A = Adj_list(N,data_graph);

    nodes.push_back(Node(&A));
    root = &(nodes.back());

    /*
     * Initialisation step, here init is worse case (IRM model),
     * another appoarch is a binary tree (TBI)
     */
    // Add a new Node for each leaf and add is as a child of root
    for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
        nodes.push_back(Node(&A,*it));
        root->addChild(&(nodes.back()));
    }
    root->updateNumInternalNodes();
}

/**
* Special test constructor
* (First element in tree_struct_graph must contain root)
*/
Tree::Tree(list<pair<int,int>> data_graph, list<pair<int,int>> tree_struct_graph,
           vector<int> data_leaf_relation) {

    // - Construct adj list from data_graph
    int N = (int) data_leaf_relation.size();
    A = Adj_list(N, data_graph);

    // 1.0 Construct the tree from tree_struct_graph
    //Get first relation parrent --> child, assumption the root is first
    pair<int,int> element = tree_struct_graph.front();
    tree_struct_graph.pop_front();

    nodes.push_back(Node(&A,element.first));
    root = &(nodes.back());

    //Add the first node as a child
    Node new_child = Node(&A,element.second);
    //new_child.setParent(&root);

    nodes.push_back(new_child);
    root->addChild(&(nodes.back()));

    //Insert parrent-->child relations for the rest
    while (!tree_struct_graph.empty()) {
        //Get next relation parrent --> child
        element = tree_struct_graph.front();
        tree_struct_graph.pop_front();

        Node * parent = this->getNode(element.first);
//        cout << "Is null?: " << (parent == nullptr) << endl; // DEBUG
//        cout << "Found node: " << parent->getLeafId() << endl; // DEBUG
        new_child = Node(& A,element.second);

        Node* existing_nodeP = this->getNode(element.second);
        if (existing_nodeP==nullptr) {
            nodes.push_back(new_child);
            parent->addChild(& (nodes.back()));
        } else{
            parent->addChild(existing_nodeP);
        }
//        cout << "First: " << element.first << " Second: " << element.second << endl; // DEBUG

    }

    //Updates internal count.
    root->updateNumInternalNodes();


    /*
     * For each leaf node, correct the leaf ID, so it correspond to the data ID
     *  each internal node is assigned a unique negative number.
     */
    int new_id =-1;
    root->setLeafId(new_id);

    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        if (it->isInternalNode()){ //Internal node
            new_id--;
            it->setLeafId(new_id);

        } else { //Leaf node
             //Find what the fake_id corresponds to in real id
            int fake_id = it->getLeafId();
            it->setLeafId(data_leaf_relation[fake_id]);
        }
    }

}
/**
 * Finds a specific node (characterized by an unique id) and
 * returns a pointer to this node or nullptr if it isn't pressent
 * (Basic implementation!)
 */
Node * Tree::getNode(int leaf_id){
    //Iterates over all internal and leaf nodes
    if (leaf_id == 0) { // is root?
        return root;
    }
    for(list<Node>::iterator it = nodes.begin();
        it != nodes.end(); it++){

        if (it->getLeafId() == leaf_id) {
            return &(* it);
        }

    }
    return nullptr;
}

list<pair<int, int>> Tree::getCountsAll(){
    return root->getCountsAll();
}


/**
* Get random node in tree (recursive operation)
* - Chooses internal nodes with weight 2 and leaves with weight 1
* - calls getRandomChild from node-class
*/

Node * Tree::getRandomNode() {
    return root->getRandomDescendant();
}

/**
 *
 */
Tree Tree::regraft(){
    Tree new_Tree = *this;
// TODO: finish the regrafting
//    Node * scion = new_Tree.getRandomNode();
//    new_Tree.cutSubtree(scion);
//
//    Node * stock = new_Tree.getRandomNode();
//    new_Tree.insertSubtree(stock, scion, true);

    return new_Tree;
}

string Tree::toString(){
    string sAdj = A.toString();
    return root->toString() + sAdj ;
}


/**
* Evaluating log-likelihood x prior of tree
*/

double Tree::evaluateLogLikeTimesPrior(double alpha, double beta, int rho_plus, int rho_minus){
//    double root_node_contribution = this->root->evaluateNodeLogLike(alpha,beta,rho_plus,rho_minus);
//    double root_subtree_contribution = this->root->evaluateSubtreeLogLike(alpha
//                                                    ,beta,rho_plus,rho_minus);
//
//    return root_node_contribution + root_subtree_contribution;
    return this->root->evaluateSubtreeLogLike(alpha,beta,rho_plus,rho_minus);
}

void Tree::cutSubtree(Node * scionP){
    Node * parentP = scionP->getParent();
    parentP->removeChild(scionP);
    scionP->setParent(nullptr);
    // TODO: remove obsoleted internal nodes
}

Node * Tree::getRoot(){
    return root;
}

void Tree::insertSubtree(Node * stockP, Node * scionP, bool asChild){
    if(asChild){
        stockP->addChild(scionP);
    }else{

        // TODO: Replacing the root node.

        // cut the stock
        Node * parent = stockP->getParent();
        parent -> removeChild(stockP);

        // add a new parent
        nodes.push_back(Node());
        Node * new_parent = &(nodes.back());
        parent->addChild(new_parent);

        // add graft both stock and scion to the new parent
        new_parent->addChild(stockP);
        new_parent->addChild(scionP);

    }
}
