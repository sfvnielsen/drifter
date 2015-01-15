% This program extracts an edge list from a pajek text (.net) file
% INPUT: .net (or .txt) filename, n - number of nodes in the graph
% OUTPUT: edge list, mx3, m - # edges
% GB, October 7, 2009

function el=pajek2edgeL(filename,n)

%[e1,e2,e3] = textread(filename,'%6d%6d%6d','headerlines',n+2);
%el=[e1,e2,e3];

% ALTERNATIVE: Not using the number of nodes as an input:
f=fopen(filename,'r');
C = textscan(f, '%s');
c=C{1};
ind_edges=find(ismember(c, '*Edges')==1);

if length(ind_edges)==0
    ind_edges = find(ismember(c, '*Arcs')==1);
end

if length(ind_edges)==0
    error('No edges found in file')
end

e1=[]; e2=[];
for cc=ind_edges:length(c)
    % two indices are edges, one is weight and again
    if mod(ind_edges,2)==mod(cc,2) % this is edge weight
        e2=[e2, str2num(c{cc})];
    elseif mod(ind_edges,2)==mod(cc-1,2) % this is node 1
        e1=[e1, str2num(c{cc})];
    end
end

el=[e1',e2'];