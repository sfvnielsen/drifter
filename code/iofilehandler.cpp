/**
 *  Project:        Parmugit
 *  Class:          IoFileHandler
 *  Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
 *  Affiliation:    Technical University of Denmark
 *  Date:           January 2015
 */

#include "iofilehandler.h"
#include <fstream>
#include <stdexcept>
#include <dirent.h>

using namespace std;


/**
* Default dummy constructor - default is a read-only object
*/
IoFileHandler::IoFileHandler(): write(0) {}

/**
* New filehandler - sets filename, and if file is used for read or write
*/
IoFileHandler::IoFileHandler(string new_filename): filename(new_filename) {
 // check that filename is valid (if read is performed)
    ifstream inStream(filename);
    if (!inStream)
    {
        throw runtime_error("IOHANDLER:  Could not read desired file");
    }
    this->read_graph();
}

/**
 * Possiblity of reading test case
 * Only used for testing/debugging
 */
IoFileHandler::IoFileHandler(string new_filename, bool test):
filename(new_filename) {
    ifstream inStream(filename);
    if (!inStream)
    {
        throw runtime_error("IOHANDLER:  Could not read desired file");
    }
    if (test) {
        this->read_test_case();
    }else {
        this->read_graph();
    }
}


/**
 Trivial get/set functions
*/

list<pair<int,int>> IoFileHandler::getDataEl() {
    return data_edge_list;
}

list<pair<int,int>> IoFileHandler::getTreeEl() {
    return tree_edge_list;
}

vector<int> IoFileHandler::getLeafDataRelation() {
    return leaf_data_relation;
}

string IoFileHandler::getFilename() {
    return filename;
}

double IoFileHandler::getAlpha() {
    return alpha;
}

double IoFileHandler::getBeta() {
    return beta;
}

int IoFileHandler::getRhoPlus() {
    return rho_plus;
}

int IoFileHandler::getRhoMinus() {
    return rho_minus;
}

double IoFileHandler::getLlike() {
    return llike;
}


void IoFileHandler::setDataEl(list<pair<int,int>> new_data_el){
    data_edge_list = new_data_el;
}

void IoFileHandler::setTreeEl(list<pair<int,int>> new_tree_el) {
    tree_edge_list = new_tree_el;
}

void IoFileHandler::setLeafDataRelation(vector<int> new_leafdata_rel) {
    leaf_data_relation = new_leafdata_rel;
}

void IoFileHandler::setFilename(string new_filename) {
    if (!write) {
        ifstream inStream(new_filename);
        if (!inStream)
        {
            throw runtime_error("IOHANDLER: Could not read desired file");
        }
    }
    filename = new_filename;
}

void IoFileHandler::setAlpha(double new_alpha) {
    alpha = new_alpha;
}

void IoFileHandler::setBeta(double new_beta) {
    alpha = new_beta;
}

void IoFileHandler::setRhoPlus(int new_rp) {
    alpha = new_rp;
}

void IoFileHandler::setRhosMinus(int new_rm) {
    alpha = new_rm;
}

void IoFileHandler::setLlike(double new_llike) {
    llike = new_llike;
}
/**
 --- End of trivial get/set functions
*/


/**
*Reads in real network data
*   - Assumes that file is an edge list
*   - If data-field in object is non-empty function does nothing and returns 1
*/
int IoFileHandler::read_graph() {
    if(!data_edge_list.empty()) {
        throw runtime_error("IOHANDLER: Something is already in the data field");
        return 1;
    }

    ifstream inStream(filename); //

    // Reading edge list
    int e1,e2;
    while (inStream >> e1 >> e2) {
        pair<int,int> edge (e1,e2);
        data_edge_list.push_back(edge);
    }
    return 0;
}


/**
* Reads test cases from MATLAB testing framework
*/
int IoFileHandler::read_test_case() {
    if(!data_edge_list.empty() | !tree_edge_list.empty() |
       !leaf_data_relation.empty()) {
        throw runtime_error(
            "IOHANDLER: Something is already in the one of the fields");
        return 1;
    }

    ifstream inStream(filename);
    // Reading number of data nodes and edges
    int number_data_nodes,number_data_edges;
    inStream >> number_data_nodes;
    inStream >> number_data_edges;

    // Reading edge list
    int e1,e2;
    for (int i=0; i!=number_data_edges; ++i)
    {
        inStream >> e1 >> e2;
        pair<int,int> edge (e1,e2);
        data_edge_list.push_back(edge);
    }

    // Reading in number of nodes in entire tree
    int number_nodes_tree;
    inStream >> number_nodes_tree;

    // Reading tree edge list (Parent->Child)
    for (int i=0; i!=number_nodes_tree-1; ++i)
    {
        inStream >> e1 >> e2;
        pair<int,int> edge (e1,e2);
        tree_edge_list.push_back(edge);
    }

    // Reading in relation between tree leaves and data nodes
    // - A vector of maximal length (N*2) is made
    leaf_data_relation = vector<int>(number_data_nodes*2);

    for (int i=0; i!=number_data_nodes; ++i)
    {
        inStream >> e1 >> e2;
        //e1 is the leaf id, i.e. "fake_id"
        //e2 is the id given in the data
        leaf_data_relation [e1] = e2;
    }

    // Reading in hyperparameters
    inStream >> alpha >> beta >> rho_plus >> rho_minus;

    // Reading in true log-likelihood
    inStream >> llike;
    return 0;
}


/**
*Print function
*/
string IoFileHandler::toDataEdgeListString() {
    string result = "";
    for (auto it = data_edge_list.begin(); it != data_edge_list.end(); ++it) {
        result += "(" + to_string(it->first) + "," + to_string(it->second) +")\n";
    }
    return result;
}
