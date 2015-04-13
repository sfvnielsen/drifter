/**
 *  Project:        Parmugit
 *  Class:          Adj_list
 *  Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
 *  Affiliation:    Technical University of Denmark
 *  Date:           January 2015
 */

#include "adjacency_list.h"
#include <string>
#include <vector>
#include <list>
#include <random>
#include <cassert>
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

    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(0.95); // one tenth of possible links unobserved

    //Construct adjacency_matrix
    adjacency_matrix = vector<vector<link>>(N,vector<link>(N,link()));

    for(int i = 0; i < N; i++){
        for(int j = i+1; j < N; j++){
            bool observed = d(gen);
            adjacency_matrix[i][j].observed = observed;
            adjacency_matrix[j][i].observed = observed;
        }
    }

    for (auto it = edge_list.begin();
         it != edge_list.end(); it++){
            adjacency_matrix[it->first][it->second].connected = true;
            adjacency_matrix[it->second][it->first].connected = true;
    }
}

/**
 * Query the connection between two nodes.
 */
bool Adj_list::isConnected(int current, int target){
        return adjacency_matrix[current][target].connected;
}

/**
 * Query the connection between two nodes.
 */
bool Adj_list::isObserved(int current, int target){
        return adjacency_matrix[current][target].observed;
}

/**
 * Query the connection between two sets of nodes.
 */
std::pair<int,int> Adj_list::getCounts(std::vector<int> * LAP, std::vector<int> * LBP){
    int nLinks = 0; // Number of links between the groups.
    int nnLinks = 0; // Number of nonLinks between the groups.

    for (auto fst = LAP->begin(); fst != LAP->end(); fst++) {
        for (auto snd = LBP->begin(); snd != LBP->end(); snd++) {
            if(isObserved(*fst,*snd)){
                if(isConnected(*fst,*snd)){
                    nLinks++;
                }else{
                    nnLinks++;
                }
            }
        }
    }

    pair<int, int> result (nLinks,nnLinks);
    return result;
}

/**
 * Query the connection between two sets of nodes.
 */
std::pair<int,int> Adj_list::getUnknownCounts(std::vector<int> * LAP, std::vector<int> * LBP){
    int nLinks = 0; // Number of links between the groups.
    int nnLinks = 0; // Number of nonLinks between the groups.

    for (auto fst = LAP->begin(); fst != LAP->end(); fst++) {
        for (auto snd = LBP->begin(); snd != LBP->end(); snd++) {
            if(!isObserved(*fst,*snd)){
                if(isConnected(*fst,*snd)){
                    nLinks += 1;
                }else{
                    nnLinks += 1;
                }
            }
        }
    }

    pair<int, int> result (nLinks,nnLinks);
    return result;
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
 * Make a string of all links
 */
string Adj_list::toGexf(){
    int edgeId = 0;
    string s = "<edges>\n";
    for(int i = 0; i != (int) adjacency_matrix.size(); i++) {
        for(int j = i; j != (int) adjacency_matrix.size(); j++) {
            if(isConnected(i,j) ){
                s += "<edge id=\"" + to_string(edgeId) + "\" source=\"" + to_string(i) + "\" target=\"" + to_string(j) + "\" />\n";
                edgeId++;
            }
        }
    }
    s += "</edges>\n";
    return s;
}
/**
 * Get the number of nodes.
 */
int Adj_list::getSize(){
    return (int) adjacency_matrix.size();
}


