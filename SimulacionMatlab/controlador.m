function u = controlador(x)

theta0 = 1;
masa = 1;
g = 9.81;

kp = 4;
kd = 4;

u = masa * g * sin(x(1)) + kp * x(1) + kd * (x(2) - x(3)) + 0.2 * x(3);