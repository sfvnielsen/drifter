DRIFTER
======

Dynamic Regrafting for InFinite TrEestructured Relationalmodel - Hierarchical structure in relational data

In this project, carried out at Technical University of Denmark, we implemented a hierarchical relational model in the style of [Schmidt et. al.][1], but with a Bernoulli-likelihood (and appropriate Beta-priors) for simple undirected relational data. 

All of the implementation was done in C++.

Installation and running
-------
The code includes a makefile. To install run "make" in the '/code' directory and this will create the executable "drifter".


From commandline (in the code directory) this can be run by invoking 
<code>
Syntax:
   drifter datafile [#Iterations = 100] [#Thinning = 10] [output-dir] [holdoutFraction] [sample hyperparameters?]
</code>

Ex. drifter data/network.txt 10000 10 results/outfiles 0.05 0

"datafile" is the path to your data (formatted as an edgelist)
Thinning how often you want to save a sample in the chain (after burn-in) (set to 1 to keep all)
"output-dir" is where the results will be written (NB! Needs to already exist when calling the executable). Burn-in will be performed equal to half of the number of iterations specified.


[1]: http://ieeexplore.ieee.org/xpl/login.jsp?tp=&arnumber=6844498&url=http%3A%2F%2Fieeexplore.ieee.org%2Fxpls%2Fabs_all.jsp%3Farnumber%3D6844498


