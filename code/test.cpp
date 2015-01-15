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

using namespace std;

int main()
{
    cout << "!!!Hello Test-World!!!" << endl;

    // Tolerance on likelihood result
//    double epsilon = 1e-6;
//
//    string dir_str =  "test/test_files/";
//    // For each test file in test_files directory
//    DIR *dir;
//    struct dirent *ent;
//    if ((dir = opendir (dir_str.c_str())) != NULL)
//    {
//        /* For all files and directories within directory */
//        int number_files_read = 0;
//
//        // Collect all test cases with same number of data nodes
//        list<double> llike_all_n3, llike_all_n3_true;
//        list<double> llike_all_n4, llike_all_n4_true;
//        // Test statistics
//        int num_passed = 0;
//        int num_failed = 0;
//
//        while ((ent = readdir (dir)) != NULL)
//        {
//            string test_file_name(ent->d_name);
//            if (test_file_name == "." || test_file_name == ".." || test_file_name.back() == '~' || test_file_name[0] == '.')
//            {
//                cout << "Skipping non-interesting stuff.." << endl;
//                continue;
//            }
//            // Read in test file
//            cout << "Reading file '" << test_file_name << "..." << endl;
//            bool should_write = 0;
//            IoFileHandler test_case(dir_str+test_file_name,should_write);
//            test_case.read_test_case();
//
//            list<pair<int,int>> data_edge_list = test_case.getDataEl();
//            list<pair<int,int>> tree_edge_list = test_case.getTreeEl();
//            vector<int> leaf_data_relation = test_case.getLeafDataRelation();
//            double alpha = test_case.getAlpha();
//            double beta = test_case.getBeta();
//            int rho_plus = test_case.getRhoPlus();
//            int rho_minus = test_case.getRhoMinus();
//            double llike_true = test_case.getLlike();
//
//
//            //Format into tree-class structure (approriate constructors)
//            Tree test_tree = Tree(data_edge_list, tree_edge_list, leaf_data_relation );
//
//            // Perform tests - evaluate likelihood of tree
//            cout << "Local-Likelihood test..." << endl << flush ;
//
//            double llike_test = test_tree.evaluateLogLikeTimesPrior(alpha,beta,rho_plus,rho_minus);
//
//
//            if (abs(llike_test-llike_true)<epsilon)
//            {
//                cout << "PASSED!" << endl;
//                num_passed++;
//            }
//            else
//            {
//                cout << "FAILED with difference:  " << llike_true-llike_test << endl;
//                num_failed++;
//            }
//
//            // Save result likelihood result
//            int number_data_nodes = ( (int) test_case.getLeafDataRelation().size())/2;
//            if (number_data_nodes == 3)
//            {
//                llike_all_n3.push_back(llike_test);
//                llike_all_n3_true.push_back(llike_true);
//            }
//            if(number_data_nodes == 4)
//            {
//                llike_all_n4.push_back(llike_test);
//                llike_all_n4_true.push_back(llike_true);
//            }
//            number_files_read++;
//        }
//        closedir (dir);
//
//        // Print test statistics
//        cout << endl << endl;
//        cout << "Number of 'local' tests performed: " << number_files_read << endl;
//        cout << "PASSES: " << num_passed << endl;
//        cout << "FAILS: " << num_failed << endl << endl;
//
//        // Marginal Likelihood Test
//        // - Sum of all test cases should be the same as true
//        cout << "Performing Marginal Likelihood Test..." << endl;
//
//        // N = 3 nodes
//        double marginal_like_test = 0;
//        for (list<double>::iterator it = llike_all_n3.begin();
//            it!=llike_all_n3.end(); ++it) {
//                marginal_like_test += exp(*it);
//        }
//
//        double marginal_like_true = 0;
//        for (list<double>::iterator it = llike_all_n3_true.begin();
//            it!=llike_all_n3_true.end(); ++it) {
//                marginal_like_true += exp(*it);
//        }
//        cout << "Marginal likelihood - 3 leaves TEST: " << marginal_like_test << endl;
//        cout << "Marginal likelihood - 3 leaves TRUE: " << marginal_like_true << endl;
//
//        // N = 4 nodes
//        marginal_like_test = 0;
//        for (list<double>::iterator it = llike_all_n4.begin();
//            it!=llike_all_n4.end(); ++it) {
//                marginal_like_test += exp(*it);
//        }
//
//        marginal_like_true = 0;
//        for (list<double>::iterator it = llike_all_n4_true.begin();
//            it!=llike_all_n4_true.end(); ++it) {
//                marginal_like_true += exp(*it);
//        }
//        cout << "Marginal likelihood - 4 leaves TEST: " << marginal_like_test << endl;
//        cout << "Marginal likelihood - 4 leaves TRUE: " << marginal_like_true << endl;
//
//
//    }
//    else
//    {
//        /* could not open directory */
//        perror ("");
//        return EXIT_FAILURE;
//    }

    cout << "------ Real data test -----" << endl;
    string data_file_name = "data/celegans_edgelist.txt";

    IoFileHandler data_file(data_file_name,0);
    data_file.read_graph();

    Tree test_tree(data_file.getDataEl()); // initialize flat tree
    cout << test_tree.toString() << endl;


    return 0;
}
