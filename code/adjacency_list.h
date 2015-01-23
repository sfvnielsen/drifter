//
//  adjacency_list.h
//  gift
//
//  Created by Jesper Hinrich on 09/01/15.
//  Copyright (c) 2015 Jesper Hinrich. All rights reserved.
//

#ifndef ADJACENCY_LIST
#define ADJACENCY_LIST

#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

class Adj_list {

public:
    // Constructors
    Adj_list();
    Adj_list(std::list<std::pair<int,int>>);

    // Querying the adjacency
    bool isConnected(int, int);
    int getSize();

    std::string toString();

private:
    // Data structure
    std::vector<std::vector<bool> > adjacency_matrix;
};


#endif /* defined(ADJACENCY_LIST) */
