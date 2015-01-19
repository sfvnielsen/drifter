clear all;
close all;
clc;

%simulated_p = importdata('loglikelihood.txt');
occ = importdata('occurences.txt');
occ = importdata('occ_500k_500k.txt');
%simulated_p = importdata('loglikelihood_1000_4N.txt');
%simulated_p = importdata('loglikelihood_1m_4N.txt');
true_p = importdata('true_loglikelihood.txt');

%% Plot
close all
hold on
plot(1:26,occ./sum(occ),'.r','MarkerSize',12);
plot(1:26,exp(true_p)./sum(exp(true_p)),'ob','MarkerSize',12)
hold off
h_legend=legend('MCMC estimate','True distr.');
set(h_legend,'FontSize',16);
xlabel('Tree (T)','FontSize',16)
ylabel('P(T|X)','FontSize',16)
%title('.9m MCMC samples'

%%
s = sort(unique(simulated_p));
t = sort(unique(true_p));

disp(length(s));
disp(length(t));

%%

for i=1:length(s);
    idx = find((s(i)-t) < 10e-5);
    lenIdx(i) = length(idx);
    if length(idx) > 0
        fprintf('Found %2.6f , with %i occurences \n',s(i),length(idx));
    else
       fprintf('L = %2.6f not found \n',s(i)); 
    end
end
fprintf('\n \n');


