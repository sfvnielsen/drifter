#include "iofilehandler.h"

using namespace std;


/**
* Default dummy constructor - default is a read-only object
*/
IoFileHandler::IoFileHandler(): write(0) {}

/**
 New filehandler - sets filename, and r/w privilege
*/

IoFileHandler::IoFileHandler(string new_filename,bool writing_priv):
write(writing_priv), filename(new_filename) {}

/**
 Trivial get/set functions
*/

list<pair<int,int>> IoFileHandler::getDataEl() {
    return data_edge_list;
}

list<pair<int,int>> IoFileHandler::getTreeEl() {
    return tree_edge_list;
}

vector<int> IoFileHandler::getDataLeafRelation() {
    return data_leaf_relation;
}

string IoFileHandler::getFilename() {
    return filename;
}


void IoFileHandler::setDataEl(list<pair<int,int>> new_data_el){
    data_edge_list = new_data_el;
}

void IoFileHandler::setTreeEl(list<pair<int,int>> new_tree_el) {
    tree_edge_list = new_tree_el;
}

void IoFileHandler::setDataLeafRelation(vector<int> new_dataleaf_rel) {
    data_leaf_relation = new_dataleaf_rel;
}

void IoFileHandler::setFilename(string new_filename) {
    filename = new_filename;
}


/**
 --- End of trivial get/set functions
*/


/**

*/
int IoFileHandler::read_graph() {


}

