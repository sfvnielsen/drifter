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
#include <boost/filesystem.hpp>
#include <dirent.h>
#include "barebones_tree.h"

using namespace std;

int main() {
	cout << "!!!Hello Test-World!!!" << endl;
    string dir_str =  "test/test_files/";
    // For each test file in test_files directory
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dir_str.c_str())) != NULL) {
        /* For all files and directories within directory */
        int number_files_read = 0;

        while ((ent = readdir (dir)) != NULL) {
            string test_file_name(ent->d_name);
            if (test_file_name == "." || test_file_name == "..") {
                cout << "Skipping . or .." << endl;
                continue;
            }
            // Read in test file
            cout << "Reading file '" << test_file_name << "'..." << endl;
            ifstream inStream(dir_str+test_file_name); //
            //ifstream inStream(ent->d_name); //
            if (!inStream)
            {
                throw runtime_error("Could not open the desired file");
            }


            // Reading number of data nodes and edges
            int number_data_nodes,number_data_edges;
            inStream >> number_data_nodes;
            inStream >> number_data_edges;

            // Reading edge list
            list<pair<int,int>> data_edge_list;
            int e1,e2;
            for (int i=0; i!=number_data_edges; ++i) {
                    inStream >> e1 >> e2;
                    pair<int,int> edge (e1,e2);
                    data_edge_list.push_back(edge);
            }

            // Reading in number of nodes in entire tree
            int number_nodes_tree;
            inStream >> number_nodes_tree;

            // Reading tree edge list (Parent->Child)
            list<pair<int,int>> tree_edge_list;
            for (int i=0; i!=number_nodes_tree-1; ++i) {
                    inStream >> e1 >> e2;
                    pair<int,int> edge (e1,e2);
                    tree_edge_list.push_back(edge);
            }

            // Reading in relation between data nodes and tree leaves
            list<pair<int,int>> data_leaf_relation;
            for (int i=0; i!=number_data_nodes; ++i) {
                    inStream >> e1 >> e2;
                    pair<int,int> edge (e1,e2);
                    data_leaf_relation.push_back(edge);
            }

            // Reading in hyperparameters
            double alpha,beta,rho_plus,rho_minus;
            inStream >> alpha >> beta >> rho_plus >> rho_minus;

            // Reading in true log-likelihood
            double llike;
            inStream >> llike;

            // DEBUG; PRINTING WHOLE EDGE LIST
            //for (list<pair<int,int>>::iterator it = data_leaf_relation.begin(); it!=data_leaf_relation.end(); ++it ){
            //    cout << "(" << it->first << "," << it->second << ")"  << endl;
            //}

            //TODO: Format into tree-class structure (approriate constructors)

            //TODO: Perform tests

            //TODO: Write out if test passed and progress

            number_files_read++;
         }
        closedir (dir);
        cout << "Number of files read: " << number_files_read << endl;

        // Print test statistics

        } else {
        /* could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }



	return 0;
}
