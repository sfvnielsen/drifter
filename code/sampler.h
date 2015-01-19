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
        void run(int L, int burnin, int thinning);
        Tree getLast();
        double getLastLikelihood();

        void writeResults(std::string);

    protected:

    private:
        // Data storage
        std::list<Tree> chain;
        std::list<double> likelihoods;
        int L;
        Adj_list adjacencyList;
        double lastLogLik;


        // Hyperparameters:
        double alpha;
        double beta;
        int rho_plus;
        int rho_minus;

};

#endif // SAMPLER_H
