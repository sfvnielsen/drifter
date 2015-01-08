%% Test on all trees with N=3 nodes
N = 3;
A = [0 1 1;1 0 1;1 1 0]; % adjacency matrix

% hypers
rho_plus = 1; rho_minus = 1;
alpha = 1/2; beta = 1/2;
T = cell(1,4); I = cell(1,4);

T{1}= [0 1 1 1]; I{1} = [2 3 4];
T{2} = [0 1 1 2 2]; I{2} = [3 4 5];
T{3} = [0 1 1 2 2]; I{3} = [4 3 5];
T{4} = [0 1 1 2 2]; I{4} = [4 5 3];

LTrue = nan(1,4);
LTrue(1) = blike(T{1},I{1},A,rho_plus,rho_minus,beta,alpha);
LTrue(2) = blike(T{2},I{2},A,rho_plus,rho_minus,beta,alpha);
LTrue(3) = blike(T{3},I{3},A,rho_plus,rho_minus,beta,alpha);
LTrue(4) = blike(T{4},I{4},A,rho_plus,rho_minus,beta,alpha);
PTrue = exp(LTrue)/sum(exp(LTrue));
P_TEST = sum(PTrue)

% Write to test txt files
file_name_base = '../test_files/likelihood_test_n3_';
hypers = [alpha,beta,rho_plus,rho_minus];
writeTestScenarios(T,I,N,A,hypers,LTrue,file_name_base);


%% Test on all trees with N=4 nodes
N = 4;
A = [0 1 0 1;1 0 1 1;0 1 0 1;1 1 1 0];

% hypers
rho_plus = 1; rho_minus = 1;
alpha = 1/2; beta = 1/2;

n = 1;
LTrue = nan(1,26);
T = cell(1,26); I = cell(1,26);
% 1111 (1)
T{1} = [0 1 1 1 1]; I{1} = [2 3 4 5]; LTrue(n) = blike(T{1},I{1},A,rho_plus,rho_minus,beta,alpha); n = n+1;
% 22 (3)
T{2} = [0 1 1 2 2 3 3]; I{2} = [4 5 6 7]; LTrue(n) = blike(T{2},I{2},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{3} = [0 1 1 2 2 3 3]; I{3} = [4 6 5 7]; LTrue(n) = blike(T{3},I{3},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{4} = [0 1 1 2 2 3 3]; I{4} = [4 6 7 5]; LTrue(n) = blike(T{4},I{4},A,rho_plus,rho_minus,beta,alpha); n = n+1;
% 13 111 (4)
T{5} = [0 1 1 2 2 2]; I{5} = [3 4 5 6]; LTrue(n) = blike(T{5},I{5},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{6} = [0 1 1 2 2 2]; I{6} = [4 3 5 6]; LTrue(n) = blike(T{6},I{6},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{7} = [0 1 1 2 2 2]; I{7} = [4 5 3 6]; LTrue(n) = blike(T{7},I{7},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{8} = [0 1 1 2 2 2]; I{8} = [4 5 6 3]; LTrue(n) = blike(T{8},I{8},A,rho_plus,rho_minus,beta,alpha); n = n+1;
% 13 12 (12)
T{9} = [0 1 1 2 2 4 4]; I{9} = [3 5 6 7]; LTrue(n) = blike(T{9},I{9},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{10} = [0 1 1 2 2 4 4]; I{10} = [3 6 5 7]; LTrue(n) = blike(T{10},I{10},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{11} = [0 1 1 2 2 4 4]; I{11} = [3 6 7 5]; LTrue(n) = blike(T{11},I{11},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{12} = [0 1 1 2 2 4 4]; I{12} = [5 3 6 7]; LTrue(n) = blike(T{12},I{12},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{13} = [0 1 1 2 2 4 4]; I{13} = [6 3 5 7]; LTrue(n) = blike(T{13},I{13},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{14} = [0 1 1 2 2 4 4]; I{14} = [6 3 7 5]; LTrue(n) = blike(T{14},I{14},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{15} = [0 1 1 2 2 4 4]; I{15} = [5 6 3 7]; LTrue(n) = blike(T{15},I{15},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{16} = [0 1 1 2 2 4 4]; I{16} = [6 5 3 7]; LTrue(n) = blike(T{16},I{16},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{17} = [0 1 1 2 2 4 4]; I{17} = [6 7 3 5]; LTrue(n) = blike(T{17},I{17},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{18} = [0 1 1 2 2 4 4]; I{18} = [5 6 7 3]; LTrue(n) = blike(T{18},I{18},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{19} = [0 1 1 2 2 4 4]; I{19} = [6 5 7 3]; LTrue(n) = blike(T{19},I{19},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{20} = [0 1 1 2 2 4 4]; I{20} = [6 7 5 3]; LTrue(n) = blike(T{20},I{20},A,rho_plus,rho_minus,beta,alpha); n = n+1;
% 112 (6)
T{21} = [0 1 1 1 2 2]; I{21} = [3 4 5 6]; LTrue(n) = blike(T{21},I{21},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{22} = [0 1 1 1 2 2]; I{22} = [3 5 4 6]; LTrue(n) = blike(T{22},I{22},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{23} = [0 1 1 1 2 2]; I{23} = [3 5 6 4]; LTrue(n) = blike(T{23},I{23},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{24} = [0 1 1 1 2 2]; I{24} = [5 3 4 6]; LTrue(n) = blike(T{24},I{24},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{25} = [0 1 1 1 2 2]; I{25} = [5 3 6 4]; LTrue(n) = blike(T{25},I{25},A,rho_plus,rho_minus,beta,alpha); n = n+1;
T{26} = [0 1 1 1 2 2]; I{26} = [5 6 3 4]; LTrue(n) = blike(T{26},I{26},A,rho_plus,rho_minus,beta,alpha); n = n+1;

P_TEST = sum(exp(LTrue)/sum(exp(LTrue)))


% write to test files
file_name_base = '../test_files/likelihood_test_n4_';
hypers = [alpha,beta,rho_plus,rho_minus];
writeTestScenarios(T,I,N,A,hypers,LTrue,file_name_base);
