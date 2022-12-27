function dx = cubli_dyn(t, x)

theta0 = 1;
masa = 1;
g = 9.81;

T = controlador(x);

dx = zeros(3,1);

dx(1) = 1 / theta0 * (x(2) - x(3));
dx(2) = masa * g * sin(x(1));
dx(3) = T;