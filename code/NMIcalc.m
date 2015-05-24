% NMI calc
true_labeling_file = 'data/haggman/labels.txt';
folder = 'long4';
path = ['results/' folder '/' ];

% True labeling
%load(true_labeling_file);

% HAGMANN
aux_variables = readtable(true_labeling_file,'ReadVariableNames',false);
N = size(aux_variables,1);
labels_Y = table2cell(aux_variables(:,2));
for i = 1:N,
    area = strsplit(labels_Y{i},' '); % Remove number
    area = area{1};
    if i < 501 %The switch between LH and RH in Hagmann data
        labels_Y{i} = strcat(area,' LH');
    else
        labels_Y{i} = strcat(area,' RH');
    end
    
end


%Convert Y labels to group idx
label_groups_Y = unique(labels_Y);
labelsY = zeros(N,1);
for j = 1:length(label_groups_Y),
    idx = strcmp(label_groups_Y{j},labels_Y);
    labelsY(idx) = j;
end

Ztrue = createAssignmentMatrix(labelsY);

% IRM Solution
mat_file = dir([path '*IRM*.mat']);
load([path mat_file(1).name])
Z_IRM = createAssignmentMatrix(z_map);

NMI_IRM = calcNMI(Ztrue,Z_IRM)

% HIRM solution
Zlayer = csvread([path 'mapStructure_leavesNlayers']);
Zlayer(end,:) = [];

for i = 1:size(Zlayer,2),
   idx = Zlayer(:,i) > 0;
   [~,min_idx] = min(Zlayer(idx,i));
   Zlayer(~idx,i) = Zlayer(min_idx,i);
end

HIRM_NMI = zeros(size(Zlayer,1),1);
for i = 1:size(Zlayer,1)
    Z_hirm = createAssignmentMatrix(Zlayer(i,:));
    HIRM_NMI(i) = calcNMI(Ztrue,Z_hirm);
end

NMI_HIRM_BEST = max(HIRM_NMI)
NMI_HIRM_WORST = min(HIRM_NMI)

