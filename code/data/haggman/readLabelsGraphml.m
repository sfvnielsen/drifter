function parseLabelsGraphml(filename)
xDoc = xmlread(filename);

allNodes = xDoc.getElementsByTagName('node');

% open a file for writing
fid = fopen('labels.txt', 'w');



for k = 0:allNodes.getLength-1
    node = allNodes.item(k);
    

    
    L = node.getElementsByTagName('data').item(2).getTextContent;
    
    fprintf(fid, '%d,  %s\n', k+1, char(L));

end

fclose(fid);
end