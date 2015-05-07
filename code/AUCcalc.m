clear

folder = 'results/haggman_2days'

load([folder '/scoresMAP.txt']);
[X,Y,T,AUCmap] = perfcurve(scoresMAP(:,2),scoresMAP(:,1),1);
%plot(X,Y);
%hold all
load([folder '/scoresEnsemble.txt']);
[X,Y,T,AUCensemble] = perfcurve(scoresEnsemble(:,2),scoresEnsemble(:,1),1);
%plot(X,Y);
%legend 'MAP' 'Ensemble'
disp(['MAP AUC: ' num2str(AUCmap)]);
disp(['Ensemble AUC: ' num2str(AUCensemble)]);