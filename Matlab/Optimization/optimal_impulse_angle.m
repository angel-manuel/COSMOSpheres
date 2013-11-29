function [ret] = optimal_impulse_angle()
mrua_x = sym('x = (imp*cos(a)/2)*t^2 + vx*t');
s_t = solve(mrua_x, 't')
mrua_y = subs(sym('y = (imp*sin(a)/2)*t^2 + vy*t'), 't', s_t(2));
dyda = diff(mrua_y, 'a');
ret = mrua_y;
end