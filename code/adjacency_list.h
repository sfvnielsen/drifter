//
//  adjacency_list.h
//  gift
//
//  Created by Jesper Hinrich on 09/01/15.
//  Copyright (c) 2015 Jesper Hinrich. All rights reserved.
//

#ifndef __gift__adjacency_list__
#define __gift__adjacency_list__

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <iomanip> //Setpresision
#include <string>
#include <vector>
#include <math.h>
#include <sys/time.h>	//Time taking Mac OS X

#include "dotRead.h"
#include "dijkstras.h"
#include <ilcplex/ilocplex.h>


class adjacency_list {
    typedef int vertex_t;
    typedef double weight_t;
    
public:
    <#member functions#>
    
private:
    
};


using namespace std;

struct neighbor {
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight)
    : target(arg_target), weight(arg_weight) { }
};

typedef std::vector<std::vector<neighbor> > adjacency_list_t;

/*
 * Construct Adjacency list for G
 */
adjacency_list_t constructAdjList(const vector<double >& G,
                                  const vector<double>& duals,
                                  const int & N,
                                  const bool& C,
                                  const int & numCommodities){
    adjacency_list_t adjacency_list(N);
    int count = 0;
    if (C){
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                if (G[i*N+j] >= 0){
                    adjacency_list[i].push_back(neighbor(j,G[i*N+j]));
                    count++;
                }
            }
        }
    } else{
        //Should be changed so it only goes though the arc which exist
        // that information is available after the first iteration.
        int nextInLine = 0;
        cout << "Graph: Which dual range is selected?" << endl;
        
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                if (G[i*N+j] >= 0){
                    if (G[i*N+j]-duals[C+nextInLine] < 0){
                        cout << "************ NEGATIVE VALUE IN ADJ LIST" <<
                        "************" << endl;
                        //Make the program quit!
                    } else {
                        adjacency_list[i].push_back(neighbor(j,G[i*N+j]-duals[numCommodities+nextInLine]));
                    }
                    nextInLine++;
                    
                }
            }
        }
    }
    return adjacency_list;
}


#endif /* defined(__gift__adjacency_list__) */
