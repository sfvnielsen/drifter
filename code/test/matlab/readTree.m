function [T,I] = readTree(filename)
% Reads tree file from parmugit code
f = fopen(filename, 'r');
Tf = textscan(f,'%[^\n]',1); T = str2num(Tf{1}{1}); 
If = textscan(f,'%[^\n]',1); I = str2num(If{1}{1});
fclose(f);
end

