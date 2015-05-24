function Z = createAssignmentMatrix(z)
% Function creates assignment matrix of size k x N
% k is the number of different labels in partioning z and N is the number
% of data points. Each column of output Z cotains only a single 1 the rest
% of the entries are zero.

[~,~,zz] = unique(z);
Z = zeros(max(zz),length(zz));
for i = 1:length(zz) 
    Z(zz(i),i) = 1;
end
%eof
end