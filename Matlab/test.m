[T, STATE] = ode45(@control, [0 100], [0 0]);
X = STATE(:, 1);
V = STATE(: ,2);

plot(T, X, T, V);
legend('X', 'V');