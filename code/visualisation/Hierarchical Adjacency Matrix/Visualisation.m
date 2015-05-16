clear; close all; clc
%Minimum group size, splits into smaller groups are ignored
% (Easier visualisation)
min_group_size = 10; 
football = false;true;
irm_results = true;%true;

if football,
    problemName = '../../results/football_20m/';
else
    problemName = '../../results/long4/'; %Four day Hagmann
end

%%
leavesNlayers = csvread(strcat(problemName,'mapStructure_leavesNlayers'));
leavesOrder  = csvread(strcat(problemName,'mapStructure_leavesOrder'));
adj = csvread(strcat(problemName,'mapStructure_sortedAdjMatrix'));

leavesOrder=leavesOrder+1; %One index

if irm_results
   %Reorder the adj matrix into the original input order
   [~,idx]=sort(leavesOrder);
   adj = adj(idx,idx);
   data = [];
   if football,
        data = load(strcat(problemName,'footballIRM_run1_map.mat'));
   else
        data = load(strcat(problemName,'hagmannIRM_run2_map.mat')); 
   end
   [leavesNlayers, idx] = sort(data.z_map);
   leavesNlayers = leavesNlayers';
   leavesOrder = data.leavesOrder(idx)+1; %Hagmann and Football is 0 indexed
   
   %Reorder the adj matrix into the IRM order (groups are placed by
   %eachother)
   adj = adj(leavesOrder,leavesOrder);    
end


[depth, N] = size(leavesNlayers);


%% Assign text labels
if football,
    aux_variables = readtable('football_labels.csv','ReadVariableNames',false);
    labels_Y = table2cell(aux_variables(:,2)); %zero or one index TODO
    labels_X = table2cell(aux_variables(:,3));
else
    aux_variables = readtable('labels.txt','ReadVariableNames',false);
    labels_Y = table2cell(aux_variables(:,2));
    labels_X = labels_Y; % To remove the numbering
    for i = 1:N,
       area = strsplit(labels_Y{i},' '); % Remove number
       area = area{1};
       if i > 500 %The switch between LH and RH in Hagmann data
            labels_X{i} = 'Left Hemisphere';
            labels_Y{i} = strcat(area,' LH');
       else
           labels_X{i} = 'Right Hemisphere';
           labels_Y{i} = strcat(area,' RH');
       end
       
    end
    
end

%Convert X labels to group idx
label_groups_X = unique(labels_X);
labelsX = zeros(N,1);
for j = 1:length(label_groups_X),
    idx = strcmp(label_groups_X{j},labels_X);
    labelsX(idx) = j;
end
%Convert Y labels to group idx
label_groups_Y = unique(labels_Y);
labelsY = zeros(N,1);
for j = 1:length(label_groups_Y),
    idx = strcmp(label_groups_Y{j},labels_Y);
    labelsY(idx) = j;
end

%% Permute labels to HIRM solution
labelsX = labelsX(leavesOrder);
labelsY = labelsY(leavesOrder);
labels_X = labels_X(leavesOrder);
labels_Y = labels_Y(leavesOrder);


%% Illustration
figure(1); 
%subplot 121; 
imagesc(leavesNlayers)
title('Hierarchical Tree Structure')
xlabel('Leaves')
ylabel('Depth')
colormap parula%lines
%set(gca,'YTick',1:2:N,'YLim',[1 N]);
%set(gca,'YTickLabel',label_node)
%%
idx=randperm(N);
[~,idx]=sort(leavesOrder);
figure(2); subplot 121; imagesc(adj(idx,idx)); axis equal
title('Hagmann sortering')
subplot 122; imagesc(adj); axis equal
title('HIRM sortering')

%% Padd leaves
% Column wise, (A column stores the group of a node across layers)
% If a column contains 0, that means that the node has already become
%   a leaf in a higher layer. So the group of this node since it became a
%   leaf, should be the group i initially belong to
figure(3);
subplot 121; imagesc(leavesNlayers); title('Before leaf padding')
for i = 1:size(leavesNlayers,2),
   idx = leavesNlayers(:,i) > 0;
   [~,min_idx] = min(leavesNlayers(idx,i));
   leavesNlayers(~idx,i) = leavesNlayers(min_idx,i);
end
subplot 122; imagesc(leavesNlayers); title('After leaf padding')

%% Adjust for min group size (split size)
%
%
if ~irm_results,
    for i = 2:size(leavesNlayers,1),    
        group_idx = unique(leavesNlayers(i,:));
        G = length(group_idx);
        idx = logical(zeros(G,N));
        for g = 1:G,
            idx(g,:) = leavesNlayers(i,:) == group_idx(g);
            if sum(idx(g,:)) < min_group_size,
               parent_group = unique(leavesNlayers(i-1,idx(g,:)) ); %The above group
               parent_group = parent_group(1);
               leavesNlayers(i:end,idx(g,:)) = parent_group;
            end

        end
    end
end
%%Place all members of a group next to eachother
[~, idx] = sort(leavesNlayers(end,:));
leavesNlayers = leavesNlayers(:,idx);
labels_Y = labels_Y(idx);
labels_X = labels_X(idx);
labelsX = labelsX(idx);
labelsY = labelsY(idx);
leavesOrder = leavesOrder(idx);
adj = adj(idx,idx);

figure(4); 
subplot 121; 

imagesc(leavesNlayers)
title('Hierarchical Tree Structure'); xlabel('Leaves'); ylabel('Depth')
colormap parula
%% Remove identical layers
toKeep = logical(ones(depth,1));
for i = 2:depth
    toKeep(i) = norm(leavesNlayers(i-1,:)-leavesNlayers(i,:)) ~= 0;  
end
leavesNlayers = leavesNlayers(toKeep,:);
subplot 122; 

imagesc(leavesNlayers)
title('Hierarchical Tree Structure'); xlabel('Leaves'); ylabel('Depth')
colormap parula%colorcube

%% Visualize hierarchies
img = ones(N,N,1)*0;
for i = 1:size(leavesNlayers,1)
    %Finds the groups
    group_idx = unique(leavesNlayers(i,:));
    bgColor = img;
    
    G = length(group_idx);
    colorsRange = linspace(20,N,G);%ceil(((1:G)/G)*2000);
    %colorsRange = [colorsRange(1:2:end) colorsRange(2:2:end)];
    %Plot diagonal and find group index
    idx = logical(zeros(G,N));
    for g = 1:G,
        idx(g,:) = leavesNlayers(i,:) == group_idx(g);
        %BG
        bgColor(idx(g,:),idx(g,:)) = colorsRange(g);%+20;
    end
    
    for g1 = 1:G,
        for g2 = g1:G
            bgColor(idx(g1,:),idx(g2,:)) = N-colorsRange(g1)+colorsRange(g2)/2;
            bgColor(idx(g2,:),idx(g1,:)) = N-colorsRange(g1)+colorsRange(g2)/2;
        end
    end
    
    %Links
    bgColor = bgColor.*~adj+adj*(-N/10);%colorsRange(g);
    
    hFig = figure;
    set(hFig, 'Position', [0 0 800 700])
    imagesc(bgColor);
    colormap copper
    
    if irm_results,
        title(sprintf('Structure found by IRM (%i groups)',G),'fontsize',14)
    else
        title(sprintf('Structure found by HIRM, %2i. layer (%i groups)',i,G),'fontsize',14)
    end
    
    
    
    labX = {};
    labY = {};
    labXpos = [];
    for g = 1:G
        labXpos(g) = mean(find(idx(g,:))); 
        [names dist] =getLabelDistribution(labelsX(idx(g,:)),label_groups_X);
        labX{g} = sprintf('%s (%3.2f%%)',names{1},dist(1)*100);
        
        [names dist] =getLabelDistribution(labelsY(idx(g,:)),label_groups_Y);
        labY{g} = sprintf('%s (%3.2f%%)',names{1},dist(1)*100);
    end 
    
    set(gca,'XTick',labXpos,'XLim',[1 N]);
    set(gca,'XTickLabel',labX);
    set(gca,'XTickLabelRotation',50);

    if ~football
        set(gca,'YTick',labXpos,'YLim',[1 N]);
        set(gca,'YTickLabel',labY);
    else
        set(gca,'YTickLabel',[]);
    end
end
