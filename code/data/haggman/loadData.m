clear
close all
clc

readLabelsGraphml('Networks/subjectA1.graphml')
%%
A1 = readGraphml('Networks/subjectA1.graphml');
B = readGraphml('Networks/subjectB.graphml');
C = readGraphml('Networks/subjectC.graphml');
D = readGraphml('Networks/subjectD.graphml');
E = readGraphml('Networks/subjectE.graphml');
%%
% Sum
A = (A1 + B + C + D + E);

% transform to undirected
A = (A+transpose(A))>0;

% Diagonal removal
n = size(A,1);
A(1:n+1:n*n) = 0;
%A = A - diag(diag(A));

[i,j] = find(A);
M = [j, i];
M = M -1 ; % zero index 
%%
dlmwrite('haggman_edgelist.txt',M,'delimiter',' ');