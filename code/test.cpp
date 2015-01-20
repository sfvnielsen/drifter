//============================================================================
// Name        : test.cpp
// Author      : Søren
// Version     :
// Copyright   : It is OURS!!
// Description : Test function
//============================================================================

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <dirent.h>
#include <cmath>
#include "barebones_tree.h"
#include "iofilehandler.h"
#include "sampler.h"
#include "time.h"
#include <cstdlib>

using namespace std;

Tree debuggingTree();
void testSamplerDistribution(string,int,int);
list<pair<Tree, double>> unique_4_tree;

int main()
{
    cout << "!!!Hello Test-World!!!" << endl;

//     Tolerance on likelihood result
    double epsilon = 1e-6;

    string dir_str =  "test/test_files/";
    string mat_out_dir = "test/mat_test/";
    // For each test file in test_files directory
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dir_str.c_str())) != NULL)
    {
        /* For all files and directories within directory */
        int number_files_read = 0;

        // Collect all test cases with same number of data nodes
        list<double> llike_all_n3, llike_all_n3_true;
        list<double> llike_all_n4, llike_all_n4_true;
        // Test statistics
        int num_passed = 0;
        int num_failed = 0;

        while ((ent = readdir (dir)) != NULL)
        {
            string test_file_name(ent->d_name);
            if (test_file_name == "." || test_file_name == ".." || test_file_name.back() == '~' || test_file_name[0] == '.')
            {
                cout << "Skipping non-interesting stuff.." << endl;
                continue;
            }
            // Read in test file
            cout << "Reading file '" << test_file_name << "..." << endl;
            bool istest = 1;
            IoFileHandler test_case(dir_str+test_file_name,istest);

            list<pair<int,int>> data_edge_list = test_case.getDataEl();
            list<pair<int,int>> tree_edge_list = test_case.getTreeEl();
            vector<int> leaf_data_relation = test_case.getLeafDataRelation();
            double alpha = test_case.getAlpha();
            double beta = test_case.getBeta();
            int rho_plus = test_case.getRhoPlus();
            int rho_minus = test_case.getRhoMinus();
            double llike_true = test_case.getLlike();

            int number_data_nodes = ( (int) test_case.getLeafDataRelation().size())/2;
            //Init adjacency list
            Adj_list adjacency_list = Adj_list(number_data_nodes, data_edge_list);

            //Format into tree-class structure (approriate constructors)
            Tree test_tree = Tree(tree_edge_list, leaf_data_relation, &adjacency_list);


            // Test of matlab format method
            string mat_out_file = mat_out_dir + test_file_name;
            test_tree.writeMatlabFormat(mat_out_file);
            // Perform tests - evaluate likelihood of tree
            cout << "Local-Likelihood test..." << endl << flush ;

            double llike_test = test_tree.evaluateLogLikeTimesPrior(alpha,beta,rho_plus,rho_minus);
            
            unique_4_tree.push_back(pair<Tree,double>(test_tree,llike_test)); //Push back all test trees (of 3 and 4 nodes)

            if (abs(llike_test-llike_true)<epsilon)
            {
                cout << "PASSED!" << endl;
                num_passed++;
            }
            else
            {
                cout << "FAILED with difference:  " << llike_true-llike_test << endl;
                num_failed++;
            }

            // Save result likelihood result
            if (number_data_nodes == 3)
            {
                llike_all_n3.push_back(llike_test);
                llike_all_n3_true.push_back(llike_true);
            }
            if(number_data_nodes == 4)
            {
                llike_all_n4.push_back(llike_test);
                llike_all_n4_true.push_back(llike_true);
            }
            number_files_read++;
        }
        closedir (dir);

        // Print test statistics
        cout << endl << endl;
        cout << "Number of 'local' tests performed: " << number_files_read << endl;
        cout << "PASSES: " << num_passed << endl;
        cout << "FAILS: " << num_failed << endl << endl;

        // Marginal Likelihood Test
        // - Sum of all test cases should be the same as true
        cout << "Performing Marginal Likelihood Test..." << endl;

        // N = 3 nodes
        double marginal_like_test = 0;
        for (list<double>::iterator it = llike_all_n3.begin();
            it!=llike_all_n3.end(); ++it) {
                marginal_like_test += exp(*it);
        }

        double marginal_like_true = 0;
        for (list<double>::iterator it = llike_all_n3_true.begin();
            it!=llike_all_n3_true.end(); ++it) {
                marginal_like_true += exp(*it);
        }
        cout << "Marginal likelihood - 3 leaves TEST: " << marginal_like_test << endl;
        cout << "Marginal likelihood - 3 leaves TRUE: " << marginal_like_true << endl;

        // N = 4 nodes
        marginal_like_test = 0;
        for (list<double>::iterator it = llike_all_n4.begin();
            it!=llike_all_n4.end(); ++it) {
                marginal_like_test += exp(*it);
        }

        marginal_like_true = 0;
        for (list<double>::iterator it = llike_all_n4_true.begin();
            it!=llike_all_n4_true.end(); ++it) {
                marginal_like_true += exp(*it);
        }
        cout << "Marginal likelihood - 4 leaves TEST: " << marginal_like_test << endl;
        cout << "Marginal likelihood - 4 leaves TRUE: " << marginal_like_true << endl;


    }
    else
    {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
                                    
    
    srand((unsigned int) time(NULL));

    
    Tree eTree = debuggingTree();
    cout << "Debugging sampler" << endl;
    
    testSamplerDistribution("test/ValidateSampler",1000000,200000);
    
    cout << "------- END -------" << endl;
    return 0;
}

/**
 * Debugging tree, useful for debugging specific situations.
 */
Tree debuggingTree(){
    pair<int,int> g1 (0,1);
    pair<int,int> g2 (1,2);
    pair<int,int> g3 (0,3);
    pair<int,int> g4 (1,3);
    pair<int,int> g5 (2,3);
    list<pair<int,int>> data_edge_list = {g1,g2,g3,g4,g5};

    int N = 4;
    Adj_list adj = Adj_list(4, data_edge_list);

    return Tree(N,&adj);
}

/**
 * Testing that the implemented sampler samples from the target distribution
 * This is very compute and memory intensive for even small problems, where
 * all the possible multi-furcating gibbs trees are know.
 */
void testSamplerDistribution(string folder,int num_samples, int num_burn){
    //Same network as used for testing that the likelihood is correct.
    pair<int,int> g1 (0,1);
    pair<int,int> g2 (1,2);
    pair<int,int> g3 (0,3);
    pair<int,int> g4 (1,3);
    pair<int,int> g5 (2,3);
    list<pair<int,int>> data_edge_list = {g1,g2,g3,g4,g5};
    
    //Removing 3-node trees from the list
    unique_4_tree.pop_front();
    unique_4_tree.pop_front();
    unique_4_tree.pop_front();
    unique_4_tree.pop_front();
    
    //Initialisation
    int N = 4;
    Adj_list adj = Adj_list(N, data_edge_list);
    Sampler sampler = Sampler(data_edge_list,0.5,0.5,1,1);;
    
    //Running sampler, num_samples
    sampler.run(num_samples);
    
    //There are exactly 26 possible trees for a 4 node network
    vector<int> num_occ = vector<int>(26,0);
    int j = 0;
    
    //Goes through each sampled tree to see which of the 26 possible trees
    // is used, in order to estimate the emperical distribution of the trees.
    for (auto it = sampler.chain.begin(); it != sampler.chain.end(); it++){
        int which_tree = 0;
        if (j > num_burn){
            //Finding the correct tree
            for (auto it_tree = unique_4_tree.begin(); it_tree != unique_4_tree.end(); it_tree++){
                if (it_tree->first.getRoot()->isEqualSubtree(it->getRoot())){
                    break;
                }
                which_tree++;
            }
            //Adds an occurence of the tree
            ++num_occ[which_tree];
        }
        j++;
    }

    // if folder doesnt exist - create it
    DIR *dir;
    if ((dir = opendir (folder.c_str())) == NULL) {
        throw runtime_error("Target directory for writing results not found");
    }
    
    // write true_likelihood and occurences
    string filename = folder + "/true_loglikelihood.txt",
           filename2 = folder + "/occurences.txt";
    ofstream out_file(filename);
    ofstream out_file2(filename2);
    
    for (auto it = unique_4_tree.begin(); it != unique_4_tree.end(); ++it){
        out_file << it->second << " ";

    }
    
    for (auto i = 0; i < num_occ.size(); i++) {
        out_file2 << num_occ[i] << " ";
    }
    
    //All loglikelihoods can also be written.
    //sampler.writeLogLikelihood(folder);
    
}

