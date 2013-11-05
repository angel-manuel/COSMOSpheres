f_score = @(d) 20.*exp(-4.*d.^2);

X = -0.64:0.01:0.64;
Y = -0.80:0.01:0.80;
Z = -0.64:0.01:0.64;

[XX, YY, ZZ] = meshgrid(X, Y, Z);

d_a = sqrt((XX-(+0.64)).^2 + (YY-(-0.8)).^2 + (ZZ-(+0.64)).^2);
S_a = f_score(d_a);

d_b = sqrt((XX-(-0.64)).^2 + (YY-(-0.8)).^2 + (ZZ-(-0.64)).^2);
S_b = f_score(d_b);

S = S_b - S_a;

contourslice(XX, YY, ZZ, S, linspace(-0.64, 0.64, 10), linspace(-0.8, 0.8, 10), linspace(-0.64, 0.64, 10), [-20 -10 -5 -2.5 -1.25 -0.625 0.625 1.25 2.5 5 10 20]);