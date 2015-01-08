function Z = lsub(X, Y)
% LSUB Numerically stable computation of log(exp(X)-exp(Y))
maxXY = max(X, Y);
Z = maxXY + log(exp(X-maxXY)-exp(Y-maxXY));
