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
#include <iterator>

class Adj_list {

public:
    // Constructors
    Adj_list();
    Adj_list(std::list<std::pair<int,int>>);

    // Querying the adjacency
    bool isConnected(int, int);
    bool isObserved(int,int);
    int getSize();
    std::pair<int,int> getCounts(std::vector<int> *, std::vector<int> *);
    std::pair<int,int> getUnknownCounts(std::vector<int> *, std::vector<int> *);

    std::string toString();
    std::string toGexf();

private:
    // Data structure
    struct link {
        bool link;
        bool observed;
    };

    std::vector<std::vector<link> > adjacency_matrix;
};


#endif /* defined(ADJACENCY_LIST) */
