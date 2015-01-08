%% Test on all trees with N=3 nodes
N = 3; 
A = [0 1 1;1 0 1;1 1 0]; % adjacency matrix

% hypers
rho_plus = 1; rho_minus = 1;
alpha = 1/2; beta = 1/2;

T1 = [0 1 1 1]; I1 = [2 3 4];
T2 = [0 1 1 2 2]; I2 = [3 4 5];
T3 = [0 1 1 2 2]; I3 = [4 3 5];
T4 = [0 1 1 2 2]; I4 = [4 5 3];

LTrue = nan(1,4);
LTrue(1) = blike(T1,I1,A,rho_plus,rho_minus,beta,alpha);
LTrue(2) = blike(T2,I2,A,rho_plus,rho_minus,beta,alpha);
LTrue(3) = blike(T3,I3,A,rho_plus,rho_minus,beta,alpha);
LTrue(4) = blike(T4,I4,A,rho_plus,rho_minus,beta,alpha);
PTrue = exp(LTrue)/sum(exp(LTrue));
P_TEST = sum(PTrue)

%% Test on all trees with N=4 nodes
N = 4; 
A = [0 1 0 1;1 0 1 1;0 1 0 1;1 1 1 0];

% hypers
rho_plus = 1; rho_minus = 1;
alpha = 1/2; beta = 1/2;

n = 1;
LTrue = nan(1,26);
% 1111 (1)
T = [0 1 1 1 1]; I = [2 3 4 5]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
% 22 (3)
T = [0 1 1 2 2 3 3]; I = [4 5 6 7]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 3 3]; I = [4 6 5 7]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 3 3]; I = [4 6 7 5]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
% 13 111 (4)
T = [0 1 1 2 2 2]; I = [3 4 5 6]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 2]; I = [4 3 5 6]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 2]; I = [4 5 3 6]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 2]; I = [4 5 6 3]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
% 13 12 (12)
T = [0 1 1 2 2 4 4]; I = [3 5 6 7]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 4 4]; I = [3 6 5 7]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 4 4]; I = [3 6 7 5]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 4 4]; I = [5 3 6 7]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 4 4]; I = [6 3 5 7]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 4 4]; I = [6 3 7 5]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 4 4]; I = [5 6 3 7]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 4 4]; I = [6 5 3 7]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 4 4]; I = [6 7 3 5]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 4 4]; I = [5 6 7 3]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 4 4]; I = [6 5 7 3]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 2 2 4 4]; I = [6 7 5 3]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
% 112 (6)
T = [0 1 1 1 2 2]; I = [3 4 5 6]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 1 2 2]; I = [3 5 4 6]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 1 2 2]; I = [3 5 6 4]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 1 2 2]; I = [5 3 4 6]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 1 2 2]; I = [5 3 6 4]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;
T = [0 1 1 1 2 2]; I = [5 6 3 4]; LTrue(n) = blike(T,I,A,rho_plus,rho_minus,beta,alpha); n = n+1;

P_TEST = sum(exp(LTrue)/sum(exp(LTrue)))
