clear
load 'out/scoresMAP.txt'
[X,Y,T,AUCmap] = perfcurve(scoresMAP(:,2),scoresMAP(:,1),1);
plot(X,Y);
hold all
load 'out/scoresEnsemble.txt'
[X,Y,T,AUCensemble] = perfcurve(scoresEnsemble(:,2),scoresEnsemble(:,1),1);
plot(X,Y);
legend 'MAP' 'Ensemble'
