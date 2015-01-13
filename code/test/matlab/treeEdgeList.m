function [TEL,LEL] = treeEdgeList(T,I)

numberOfNodes = length(T);
TEL = nan(numberOfNodes-1,2);
nodeID = 0:numberOfNodes;

for i = 2:numberOfNodes
    if nodeID(T(i)) == 0
        TEL(i-1,:) = [0,nodeID(i)];
    else
        TEL(i-1,:)= [nodeID(T(i)),i-1];
    end
end

[leaves,dataID] = sort(I-1);

LEL = [leaves',dataID'-1];

%eof
end