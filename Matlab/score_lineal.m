f_score = @(d) 20.*exp(-4.*d.^2);

D = 0:0.01:2.4159;
S = f_score(D);

plot(D, S);