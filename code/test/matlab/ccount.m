function [C, L] = ccount(T, I, varargin)
% CCOUNT Count statistics on tree
% 
% Usage:
%   function [C, L] = ccount(T, I)
%
% Arguments:
%   T : Tree structure: Vector of parent indices for each node (root=0)
%   I : Leaf-to-node index: Leaf n is node I(n) in the tree
%
%   C : Total number of leafs below node
%   L : Leaf-numbers of all children

% Copyright 2014 Mikkel N. Schmidt
% mnsc@dtu.dk, Technical University of Denmark

% Handle input arguments
if nargin==2
    C = zeros(size(T));
    L = cell(size(T));
    n = find(T==0);
elseif nargin==5
    C = varargin{1};
    L = varargin{2};
    n = varargin{3};
else
    error('Wrong number of arguments');
end

% Loop over all nodes that should be counted
for i = n
    % If node has any children
    if any(T==i)
        % Count for the children
        [C,L] = ccount(T,I,C,L,find(T==i));
        
        % Sum the number of leafs of the children
        C(i) = sum(C(T==i));
        
        % Leaf-numbers of all children
        L{i} = [L{T==i}];
        
    % If node does not have any children, i.e., is a leaf
    else
        % Number of leafs
        C(i) = 1;
        % Leaf-number of the node
        L{i} = find(I==i);
    end
end