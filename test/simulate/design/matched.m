fs = 1280;
xt = sin(2 * pi *55 * [0: 1/ fs: 127/ fs]);
xr = [zeros(1, 10) xt(1: 118)];
tau = [1: 128];
phi = [-fs / 2 + 10: fs / 128: fs / 2 - 0];
disp(phi);
M = match(xr, xt, fs, tau, phi);
mesh(phi, tau, M), axis tight;
%plot3(phi, tau, M);
xlabel('\phi');
ylabel('\tau');
pause;
