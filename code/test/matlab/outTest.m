% test of C++ output format


%% n = 3
mat_test_dir = '../mat_test/';
files = dir([mat_test_dir 'likelihood_test_n3*']);
T_c = cell(1,length(files));
I_c = cell(1,length(files));
for i = 1:length(files)
    [T_c{i},I_c{i}] = readTree([mat_test_dir files(i).name]);
end

T{1}= [0 1 1 1]; I{1} = [2 3 4];
T{2} = [0 1 1 2 2]; I{2} = [3 4 5];
T{3} = [0 1 1 2 2]; I{3} = [4 3 5];
T{4} = [0 1 1 2 2]; I{4} = [4 5 3];

% test - for each tree read, test if the configuration mathces any in T,I
assert(length(T) == length(T_c) && length(I) == length(I_c))
matches = logical(zeros(length(T),1));
for i = 1:length(T_c)
    match = false;
    for j = 1:length(T)
        match = isequal(T{j},T_c{i}) & isequal(I{j},I_c{i});
        if match
            break
        end
    end
    
    if ~match
        disp(['No match found for ' num2str(files(i).name)]);
    end
    matches(i) = match;
end

if all(matches)
   disp('All passed') 
end


%% n = 4

% read all test cases
mat_test_dir = '../mat_test/';
files = dir([mat_test_dir 'likelihood_test_n4*']);
T_c = cell(1,length(files));
I_c = cell(1,length(files));
for i = 1:length(files)
    [T_c{i},I_c{i}] = readTree([mat_test_dir files(i).name]);
end

T = cell(1,26); I = cell(1,26);
% 1111 (1)
T{1} = [0 1 1 1 1]; I{1} = [2 3 4 5];
% 22 (3)
T{2} = [0 1 1 2 2 3 3]; I{2} = [4 5 6 7];
T{3} = [0 1 1 2 2 3 3]; I{3} = [4 6 5 7];
T{4} = [0 1 1 2 2 3 3]; I{4} = [4 6 7 5];
% 13 111 (4)
T{5} = [0 1 1 2 2 2]; I{5} = [3 4 5 6];
T{6} = [0 1 1 2 2 2]; I{6} = [4 3 5 6];
T{7} = [0 1 1 2 2 2]; I{7} = [4 5 3 6];
T{8} = [0 1 1 2 2 2]; I{8} = [4 5 6 3];
% 13 12 (12)
T{9} = [0 1 1 2 2 4 4]; I{9} = [3 5 6 7];
T{10} = [0 1 1 2 2 4 4]; I{10} = [3 6 5 7];
T{11} = [0 1 1 2 2 4 4]; I{11} = [3 6 7 5];
T{12} = [0 1 1 2 2 4 4]; I{12} = [5 3 6 7];
T{13} = [0 1 1 2 2 4 4]; I{13} = [6 3 5 7];
T{14} = [0 1 1 2 2 4 4]; I{14} = [6 3 7 5];
T{15} = [0 1 1 2 2 4 4]; I{15} = [5 6 3 7];
T{16} = [0 1 1 2 2 4 4]; I{16} = [6 5 3 7];
T{17} = [0 1 1 2 2 4 4]; I{17} = [6 7 3 5];
T{18} = [0 1 1 2 2 4 4]; I{18} = [5 6 7 3];
T{19} = [0 1 1 2 2 4 4]; I{19} = [6 5 7 3];
T{20} = [0 1 1 2 2 4 4]; I{20} = [6 7 5 3];
% 112 (6)
T{21} = [0 1 1 1 2 2]; I{21} = [3 4 5 6];
T{22} = [0 1 1 1 2 2]; I{22} = [3 5 4 6];
T{23} = [0 1 1 1 2 2]; I{23} = [3 5 6 4];
T{24} = [0 1 1 1 2 2]; I{24} = [5 3 4 6];
T{25} = [0 1 1 1 2 2]; I{25} = [5 3 6 4];
T{26} = [0 1 1 1 2 2]; I{26} = [5 6 3 4];



% test - for each tree read, test if the configuration mathces any in T,I
assert(length(T) == length(T_c) && length(I) == length(I_c))
matches = logical(zeros(length(T),1));
for i = 1:length(T_c)
    match = false;
    for j = 1:length(T)
        match = isequal(T{j},T_c{i}) & isequal(I{j},I_c{i});
        if match
            break
        end
    end
    
    if ~match
        disp(['No match found for ' num2str(files(i).name)]);
    end
    matches(i) = match;
end

if all(matches)
   disp('All passed') 
end