/**
    Project:        Parmugit
    Class:          Adj_list
    Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
    Affiliation:    Technical University of Denmark
    Date:           January 2014
*/

#include "adjacency_list.h"
#include <string>
#include <vector>
#include <list>
using namespace std;

/**
 * Dummy constructor for an empty adjacency list.
 */
Adj_list::Adj_list(){}

/**
 * Constructing the adjacency matrix from an adjacency list passed as list of pairs of its.
 */
Adj_list::Adj_list(std::list<std::pair<int,int>> edge_list){

    list<int> leaves;

    // insert all the indexes from the edge list into leaves
    for (list<pair<int,int>>::iterator it = edge_list.begin(); it != edge_list.end(); it++){
        leaves.push_back(it->first);
        leaves.push_back(it->second);
    }

    // Find only the unique elements
    leaves.sort();
    leaves.unique();
    int N = (int) leaves.size();

    //Construct adjacency_matrix
    adjacency_matrix = vector<vector<bool>>(N,vector<bool>(N,false));
    for (list<pair<int,int>>::iterator it = edge_list.begin();
         it != edge_list.end(); it++){
            adjacency_matrix[it->first][it->second] = true;
            adjacency_matrix[it->second][it->first] = true;
    }
}

/**
 * Query the connection between two nodes.
 */
bool Adj_list::isConnected(int current, int target){
        return adjacency_matrix[current][target];
}

/**
 * Make a string of all links
 */
string Adj_list::toString(){
    string s = "adj:\n";
    for(int i = 0; i != (int) adjacency_matrix.size(); i++) {
        for(int j = 0; j != (int) adjacency_matrix.size(); j++) {
            s += "  ("+to_string(i)+","+to_string(j)+")\n";
        }
    }
    return s;
}

/**
 * Get the number of nodes.
 */
int Adj_list::getSize(){
    return (int) adjacency_matrix.size();
}


