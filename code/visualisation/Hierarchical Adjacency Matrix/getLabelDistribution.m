function [names procent] = getLabelDistribution(labelsID,idNames)
    uniq_Labels = unique(labelsID);
    M = length(uniq_Labels);
    
    occ = zeros(M,1);
    for i = 1:M,
       occ(i) = sum(labelsID == uniq_Labels(i)); 
    end
    
    procent = occ/sum(occ);
    %[occ] = histcounts(labels(idx),length(label_group)-1);
    %group(g).numMembers = sum(idx);
    %[~,mostFrequent] = max(occ);
    [~, idx] = sort(occ,'descend');
    procent = procent(idx);
    ids = uniq_Labels(idx);
    names = cell(M,1);
    for i = 1:M,
        names(i) = idNames(ids(i));
    end
    
end