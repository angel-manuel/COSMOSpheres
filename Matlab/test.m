ode_options = odeset('RelTol', 1e-3, 'AbsTol', 1e-3);
[T, STATE] = ode45(@control, [0 50], [0 0], ode_options);
X = STATE(:, 1);
V = STATE(: ,2);

plot(T, X, T, V);
legend('X', 'V');