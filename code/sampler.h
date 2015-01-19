#ifndef SAMPLER_H
#define SAMPLER_H

#include "barebones_tree.h"
#include <cstdlib>

class Sampler
{
    public:
        Sampler();
        Sampler(Tree T, double alpha, double beta, int rho_plus, int rho_minus);
        Sampler(std::list<std::pair<int,int>> data_graph, double alpha, double beta, int rho_plus, int rho_minus); // Initialize with the naive tree building in the adjacency matrix.
        Sampler(std::list<std::pair<int,int>> data_graph, std::list<std::pair<int,int>> tree_struct_graph,
                std::list<std::pair<int,int>> data_leaf_relation, double alpha, double beta, int rho_plus, int rho_minus); // Initialize with tree based on data, tree and data-tree relation

        virtual ~Sampler();

        void run();
        void run(int L);
        Tree getLast();
        double getLastLikelihood();
        std::list<Tree> getChain();

        void writeResults(std::string);
        void writeLogLikelihood(std::string);
        std::list<Tree> chain;
        std::list<double> likelihoods;
    protected:

    private:
        // Data storage


        int L;
        Adj_list adjacencyList;


        // Hyperparameters:
        double alpha;
        double beta;
        int rho_plus;
        int rho_minus;

};

#endif // SAMPLER_H
