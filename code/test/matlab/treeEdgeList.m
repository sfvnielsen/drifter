function [TEL,leaves] = treeEdgeList(T,I)

numberOfNodes = length(T);
TEL = nan(numberOfNodes-1,2);

for i = 2:numberOfNodes
    if T(T(i)) == 0
        TEL(i-1,:) = [0,i-1];
    else
        TEL(i-1,:)= [T(T(i)),i-1];
    end
end

leaves = sort(I-1);

%eof
end