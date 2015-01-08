function writeTestScenarios(T,I,N,A,hypers,LTrue,file_name_base)
alpha = hypers(1);
beta = hypers(2);
rho_plus = hypers(3);
rho_minus = hypers(4);

for i = 1:length(T)
    file_name = strcat(file_name_base, sprintf('%02d',i),'.txt');
    fileID = fopen(file_name,'w');
    
    %fprintf(fileID,'%s\n','Number of nodes and edges in data: ');
    DEL = adj2edgeL(triu(A)); % data edge list
    DEL = DEL(:,1:2)-1;
    fprintf(fileID,'%d %d\n', N,size(DEL,1));
    %fprintf(fileID,'%s\n','Data Edge List');
    fprintf(fileID,'%d %d\n',DEL');
    %fprintf(fileID,'%s\n','Number of nodes in tree: ');
    fprintf(fileID,'%d\n',length(T{i}));
    [PCEL,LNEL] = treeEdgeList(T{i},I{i}); % ParentChildEdgeList and LeafDataEdgeList
    %fprintf(fileID,'%s\n','Tree structure - Parent-Child edge list');
    fprintf(fileID,'%d %d\n',PCEL');
    
    %fprintf(fileID,'%s\n','Leaf->Data node pairs');
    fprintf(fileID,'%d %d\n',LNEL');
    
    
    %fprintf(fileID,'%s\n','Hyperparameters from likelihood calc');
    %fprintf(fileID,'%s\n','Order: alpha,beta,rho_plus,rho_minus');
    fprintf(fileID,'%d %d %d %d \n',alpha,beta,rho_plus,rho_minus);
    
    
    %fprintf(fileID,'%s\n','True log-likelihood');
    fprintf(fileID,'%d',LTrue(i));
    fclose(fileID);
end

%eof
end