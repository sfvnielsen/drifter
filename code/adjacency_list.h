/**
 * Project:        Parmugit
 * Class:          Adj_list
 * Created by:     Jesper L. Hinrich, Julian K. Larsen and Soeren F. V. Nielsen
 * Affiliation:    Technical University of Denmark
 * Date:           January 2015
 */

#ifndef ADJACENCY_LIST
#define ADJACENCY_LIST

#include <string>
#include <vector>
#include <list>

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
