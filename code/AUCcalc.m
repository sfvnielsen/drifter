clear
load 'out/HoldoutScores.txt'
[X,Y,T,AUC] = perfcurve(HoldoutScores(:,2),HoldoutScores(:,1),1);
plot(X,Y);