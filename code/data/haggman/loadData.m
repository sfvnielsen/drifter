clear
close all
clc

A1 = readGraphml('subjectA1.graphml');
A2 = readGraphml('subjectA2.graphml');
%%
% Average
A = (A1 + A2)/2;

% transform to undirected
A = (A+transpose(A))>0;

% Diagonal removal
n = size(A,1);
A(1:n+1:n*n) = 0;
%A = A - diag(diag(A));

[i,j] = find(A);
M = [i,j];
%%
dlmwrite('haggman_edgelist.txt',M,'delimiter',' ');