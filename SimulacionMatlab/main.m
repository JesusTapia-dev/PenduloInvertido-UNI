%% INIT
clc; close all; clear

%% Definir la dinamica
% La dinamica esta en cubli_dyn.m

tspan = [0 10];
x0 = [0.1, 0, 0];
[t,y] = ode45(@cubli_dyn, tspan, x0);


plot(t,y)
legend('\phi', 'p_{\phi}', 'p_{\psi}');