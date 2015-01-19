//
//  adjacency_list.cpp
//  gift
//
//  Created by Jesper Hinrich on 09/01/15.
//  Copyright (c) 2015 Jesper Hinrich. All rights reserved.
//

#include "adjacency_list.h"
#include <iostream>
#include <algorithm> //std::binary_search, std::sort

using namespace std;

Adj_list::Adj_list(){}

/**
 * N size of leaves
 *
 */
Adj_list::Adj_list(int N, std::list<std::pair<int,int>> edge_list){
    adjacency_list = vector<vector<int>>(N);
    
    //Construct adjacency list
    for (list<pair<int,int>>::iterator it = edge_list.begin();
         it != edge_list.end(); it++){
            addUndirectedEdge(it->first, it->second);
    }
    for (auto it = adjacency_list.begin(); it != adjacency_list.end(); it++) {
        sort(it->begin(),it->end());
    }
    
    //Construct adjacency_matrix (set the value to true if a connection
    //  exists in the adjacency list)
    adjacency_matrix = vector<vector<bool>>(N,vector<bool>(N,false));
    for (auto i = 0; i < N; i++) {
        for (auto it = adjacency_list[i].begin(); it != adjacency_list[i].end(); it++) {
            adjacency_matrix[i][*it] = true;
            adjacency_matrix[*it][i] = true;
        }
    }

};
/*
 * Adds an directed edge in the adjacency list
 */
int Adj_list::addDirectedEdge(int from, int to){
    //adjacency_list[from].push_back(neighbor(to));
    adjacency_list[from].push_back(to);
    return 0;
}

/*
 * Adds an undirected edge, by adding two opposit directed edges
 */
int Adj_list::addUndirectedEdge(int from, int to){
    addDirectedEdge(from, to);
    addDirectedEdge(to, from);
    return 0;
};
/**
 * NOT IMPLEMENTED
 */
int Adj_list::removeDirectedEdge(int from, int to){
    return 0;
};
/**
 * REQUIRES IMPLEMENTATION OF removeDirectedEdge!
 */
int Adj_list::removeUndirectedEdge(int from, int to){
    removeDirectedEdge(from, to);
    removeDirectedEdge(to, from);
    return 0;
};
/**
 * Search for an element (TODO: in a sorted list)
 */
bool Adj_list::isConnected(int current, int target){
    /* Linear search for element *
     bool existsConnection = false;
     existsConnection = find(adjacency_list[current].begin(),
                            adjacency_list[current].end(),target) != adjacency_list[current].end();
     return existsConnection;
     */


    try {
//        return adjacency_matrix[current][target];

        //Binary search for element
        return binary_search(adjacency_list[current].begin(),
                             adjacency_list[current].end(), target);
    // If something goes wrong, tell why, an make sure that the input given were valid
    } catch (exception e) {
        if (current < 0 || current > (int) adjacency_list.size()-1 ||
            target < 0 || target > (int) adjacency_list.size()-1) {
            cout << "ERROR: Trying to look up element beyond adjacency list " << endl;
            cout << "current: " << current << " , target: " << target <<
            " : adj_list_size: " << (int) adjacency_list.size() << endl;
        }
        throw e;
    }
    
};

string Adj_list::toString(){
    string s = "adj:\n";
    for(vector<list<int>>::size_type i = 0; i != adjacency_list.size(); i++) {
    /* std::cout << someVector[i]; ... */
        for (auto snd = adjacency_list[i].begin(); snd != adjacency_list[i].end(); snd++){
            s += "  ("+to_string(i)+","+to_string(*snd)+")\n";
        }
    }
    return s;
}


