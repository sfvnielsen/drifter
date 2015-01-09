//
//  adjacency_list.cpp
//  gift
//
//  Created by Jesper Hinrich on 09/01/15.
//  Copyright (c) 2015 Jesper Hinrich. All rights reserved.
//

#include "adjacency_list.h"


using namespace std;

Adj_list::Adj_list(){}

/**
 * N size of leaves
 *
 */
Adj_list::Adj_list(int N, std::list<std::tuple<int,int>> edge_list){
    //adjacency_list (N, vector<neighbor>(N));
    adjacency_list = vector<list<int>>(N, list<int>());

    for (list<tuple<int,int>>::iterator it = edge_list.begin();
         it != edge_list.end(); it++){
            addUndirectedEdge(get<0>(*it), get<1>(*it));
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
    bool existsConnection = false;
//    for (vector<neighbor>::iterator it = adjacency_list[current].begin();
//         it != adjacency_list[current].end(); it++) {
//        //See if the current element matches the target
//        //(i.e. there is an edge)
//        if ((it->id) == target) {
//            existsConnection = true;
//            break;
//        }
//    }

    list<int> L =  adjacency_list[current];

    existsConnection = find(L.begin(),L.end(),target) != L.end();

    return existsConnection;
};


