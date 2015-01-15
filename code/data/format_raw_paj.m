
input_file = 'karate/karate.paj';
output_file = 'karate_edgelist.txt';
EL = undirected_paj2edgeL(input_file);

f =  fopen(output_file,'w');
fprintf(f,'%d %d\n',(EL-1)'); % -1 because MATLAB doesnt index at zero :S
fclose(f);
