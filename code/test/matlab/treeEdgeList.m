function [TEL,LEL] = treeEdgeList(T,I)

numberOfNodes = length(T);
TEL = nan(numberOfNodes-1,2);

for i = 2:numberOfNodes
    if T(T(i)) == 0
        TEL(i-1,:) = [0,i-1];
    else
        TEL(i-1,:)= [T(T(i)),i-1];
    end
end

[leaves,dataID] = sort(I-1);

LEL = [leaves',dataID'-1];

%eof
end