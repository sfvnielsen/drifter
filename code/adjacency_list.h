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

#include <string>
#include <vector>
//#include <math.h>
#include <list>
#include <algorithm> //std::binary_search, std::sort
class Adj_list {

public:
    struct neighbor;
    std::vector<std::vector<neighbor> > adjacency_list;
    
    Adj_list();
    Adj_list(int,std::list<std::tuple<int,int>>);
    int addDirectedEdge(int, int);
    int addUndirectedEdge(int, int);
    int removeDirectedEdge(int, int);
    int removeUndirectedEdge(int, int);
    bool existsConnection(int, int);
    
    
private:

};


#endif /* defined(__gift__adjacency_list__) */
