//
//  adjacency_list.cpp
//  gift
//
//  Created by Jesper Hinrich on 09/01/15.
//  Copyright (c) 2015 Jesper Hinrich. All rights reserved.
//

#include "adjacency_list.h"


using namespace std;
struct neighbor {
    int id;
    //Potential to add a weight
    neighbor(int arg_target) : target(arg_target){}; //Its a function
};
vector<vector<neighbor>> adjacency_list;

Adj_list::Adj_list(int N, std::list<std::tuple<int,int>> edge_list){
    adjacency_list = vector<vector<neighbor>>(N);
    
    for (list<tuple<int,int>>::iterator it = edge_list.begin();
         it != edge_list.end(); it++){
        addUndirectedEdge(get<0>(*it), get<1>(*it));
    }
    
};
/*
 * Adds an directed edge in the adjacency list
 */
int addDirectedEdge(int from, int to){
    adjacency_list[from].push_back(neighbor(to));
    return 0;
}

/*
 * Adds an undirected edge, by adding two opposit directed edges
 */
int addUndirectedEdge(int from, int to){
    addDirectedEdge(from, to);
    addDirectedEdge(to, from);
    return 0;
};
/**
 * NOT IMPLEMENTED
 */
int removeDirectedEdge(int from, int to){
    return 0;
};
/**
 * REQUIRES IMPLEMENTATION OF removeDirectedEdge!
 */
int removeUndirectedEdge(int from, int to){
    removeDirectedEdge(from, to);
    removeDirectedEdge(to, from);
    return 0;
};
/**
 * Search for an element (TODO: in a sorted list)
 */
bool isConnected(int current, int target){
    bool existsConnection = false;
    for (vector<neighbor>::iterator it = adjacency_list[current].begin();
         it != adjacency_list[current].end(); it++) {
        //See if the current element matches the target
        //(i.e. there is an edge)
        if (it->id == target) {
            existsConnection = true;
            it = adjacency_list[current].end();
        }
        
    }
    return existsConnection;
};


