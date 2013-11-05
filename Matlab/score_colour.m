f_score = @(d) 20.*exp(-4.*d.^2);

X = -0.64:0.01:0.64;
Y = -0.80:0.01:0.80;

[XX, YY] = meshgrid(X, Y);

d_a = sqrt((XX-(0.64)).^2 + (YY-(-0.8)).^2);
ZZ_a = f_score(d_a);

d_b = sqrt((XX-(-0.64)).^2 + (YY-(-0.8)).^2);
ZZ_b = f_score(d_b);

ZZ = ZZ_a - ZZ_b;

mesh(XX, YY, ZZ);