/**
 *  Project:        Parmugit
 *  Class:          Tree
 *  Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
 *  Affiliation:    Technical University of Denmark
 *  Date:           January 2015
 */

#ifndef TREE_H_
#define TREE_H_

#include <utility>
#include <list>
#include <string>
#include <vector>
#include "node.h"


class Tree {

private:
    Node * rootP;
    std::list<Node> nodes;  // Don't make vector. This will invalidate pointers when reallocating.
    int nextInternalNodeId;

    Adj_list * adjacencyListP;

    int InitFlatTree(int);
    int InitBinaryTree(int);
    Node * makeNsplitTree(int, int, int);

public:
    // Constructors
    Tree();
    Tree(Adj_list * AP,double, double, int, int);
    Tree(Adj_list * AP, std::string initType,double, double, int, int);
    Tree(std::list<std::pair<int, int>>, std::string,double, double, int, int);
    Tree(std::list<std::pair<int, int>>
         , std::vector<int>, Adj_list *,double, double, int, int);


    //Rule of "5"
    //virtual ~Tree();                           // Default destructor
    Tree(Tree const &);                         // Copy constructor
    Tree& operator=(const Tree& other);         // Copy assignment constructor.
    //Tree(Tree&&) = default;                    // Move constructor
    //Tree& operator=(const Tree&) & = default;  // Copy assignment operator
    //Tree& operator=(Tree&&) & = default;       // Move assignment operator
    bool isEqual(Tree);

    // Get and set - Trivial stuff
    Adj_list * getAdjacencyListP();
    void setRootP(Node *);
    Node * getRoot();
    int getNextInternalNodeId();
    Node * addNode();
    Node * getNode(int);
    void removeNode(Node *);

    // Hyperparameters
    double alpha;
    double beta;
    int rho_plus;
    int rho_minus;

    // Regrafting
    double naive_regraft();
    double regraft();
    void regraft(int,int); // Only used for debug
    Node * getRandomScion();
    Node * getRandomStock();
    Node * cutSubtree(Node * );
    void insertSubtree(Node *, Node *, bool);
    void updateScionAndStock(Node *, Node *, Node *);

    // Evaluating likelihood
    double evaluateLogLikeTimesPrior();
    bool isLoglikeInitialised;
    void initializeLogLike();
    bool isLoglikeCorrect();

    double predAccuracy();
    std::string holdoutScores();

    // Print
    std::string toString();
    void writeMatlabFormat(std::string);
    void writeJSONFormat(std::string);
    void writeGexf(std::string);
    std::string toGexf();
};



#endif /* TREE_H_ */

