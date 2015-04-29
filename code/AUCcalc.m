clear

load 'hgm_1_5mil/scoresMAP.txt'
[X,Y,T,AUCmap] = perfcurve(scoresMAP(:,2),scoresMAP(:,1),1);
%plot(X,Y);
%hold all
load 'hgm_1_5mil/scoresEnsemble.txt'
[X,Y,T,AUCensemble] = perfcurve(scoresEnsemble(:,2),scoresEnsemble(:,1),1);
%plot(X,Y);
%legend 'MAP' 'Ensemble'
disp(['MAP AUC: ' num2str(AUCmap)]);
disp(['Ensemble AUC: ' num2str(AUCensemble)]);