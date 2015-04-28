function M = readGraphml(filename)
xDoc = xmlread(filename);

nodes = xDoc.getElementsByTagName('node');
nodeString = nodes.item(nodes.getLength-1).getAttribute('id');
nodeString = char(nodeString);
nodeString = nodeString(2:end);
N = str2num(nodeString);

M = zeros(N);

allEdges = xDoc.getElementsByTagName('edge');

for k = 0:allEdges.getLength-1
    edge = allEdges.item(k);
    
    s = edge.getAttribute('source');
    s = char(s);
    s = s(2:end);
    source = str2num(s);
    
    t = edge.getAttribute('target');
    t = char(t);
    t = t(2:end);
    target = str2num(t);
    
    edgeStrengthString = edge.getElementsByTagName('data').item(0).getTextContent;
    edgeStrength = str2num(edgeStrengthString);
    
    M(source,target) = edgeStrength;
    
end
end