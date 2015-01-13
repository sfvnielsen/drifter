function L = blike(T, I, A, rho_plus, rho_minus, beta, alpha)
% TLIKE Compute tree likelihood using Bernoulli-Beta distributions
% 
% Usage:
%   L = blike(T, I, A,rho_plus,rho_minus,beta,alpha);
%
% Arguments
%   T : Tree structure: Vector of parent indices for each node (root=0)
%   I : Leaf-to-node index: Leaf n is node I(n) in the tree
%   A : Data matrix
%   rho_plus: Prior of links between groups
%   rho_minus: Prior of non-links between groups
%   beta, alpha: Hyperparameters in GFT-prior
%   L : Log-likelihood

% Copyright 2015 Mikkel N. Schmidt (Modified by Søren Føns Vind Nielsen)
% mnsc@dtu.dk, Technical University of Denmark

% Compute number of children of all nodes
nchildren = arrayfun(@(m) sum(T==m), 1:length(T));

% Count up statistics on the tree
[C, L] = ccount(T, I);

% Check beta and alpha
if beta<=0, L = 0; return; end;
if alpha<0, L = 0; return; end;

% --- Compute the likelihood of the data ---

% Initialize log-likelihood to zero
Ll = 0;

% Find non-leaf nodes
InternalNodes = find(nchildren>0);
% For each non-leaf node
for l = 1:length(InternalNodes)
    % Find its children
    ch = find(T==InternalNodes(l));
    % Number of children
    K = length(ch);
    % For each pair of children
    for j = 1:K
        for k = j+1:K
            % Get data
            l1 = L{ch(j)};
            l2 = L{ch(k)};
            x = A(l1, l2);  x = x(:);
            % All entries from two children
            n = length(x);
            % number of links and non-links
            np = sum(x); nm = n-np;
            % Compute log-likelihood term
            Ll = Ll + betaln(np+rho_plus,nm+rho_minus) - ...
                betaln(rho_plus,rho_minus); 
        end
    end
end


% --- Compute the prior probability of the tree ---

% Prior parameters
b = beta;
a = alpha;

% Initialize log-prior probability to zero
Lp = 0;

% Log-gamma ratio function
lg = @(N,b) gammaln(N+b)-gammaln(1+b);

% For each non-leaf node
for l = find(nchildren>0)
    % Find its children
    ch = find(T==l);
    % Number of children
    K = length(ch);
    % Total number of leafs of all children
    N = C(l);
    % Number of leafs for each child
    m = C(ch);
    % Compute log-prior term for node
    if a==0 % One-parameter CRP
%        Lp = Lp + (K-1)*log(b)-lsub(lg(N,b),lg(N,0)) + sum(lg(m,0));
        Lp = Lp + (K-1)*log(b)-lsub(gammaln(N+b)-gammaln(1+b),gammaln(N)) + sum(gammaln(m));
    else % Two-parameter CRP
        % DEBUG for C++
        %prior_cont = lg(K,b/a)+(K-1).*log(a)-lsub(lg(N,b),lg(N,-a)) ...
        %        + sum(lg(m,-a));
        Lp = Lp + lg(K,b/a)+(K-1).*log(a)-lsub(lg(N,b),lg(N,-a))...
                + sum(lg(m,-a));
    end
end

% --- Posterior cx Likelihood x Prior --- 
L = Ll + Lp;
