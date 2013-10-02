%Imprime la puntación por zonas
fscore = @(dx, dy, dz) (200.*exp(-(8.*(dx+0.0625)+3.*dy+20.*(dz.^2))).*sqrt(dx + 0.0625));

X = -0.64:0.01:0.64;
Y = -0.80:0.01:0.80;
Z = -0.64:0.01:0.64;
DX = abs(X + 0.64);
DY = abs(Y - 0.80);
DZ = abs(Z + 0.64);
[XX, YY, ZZ] = meshgrid(DX, DY, DZ);

score = fscore(XX, YY, ZZ);
contourf(X, Y, max(score, [], 3), 20);
axis equal;