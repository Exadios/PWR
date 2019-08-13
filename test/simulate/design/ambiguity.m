n_max = 64;
t_s = 4;

t = [0: n_max - 1]'; z = randn(size(t));  % z is white Gaussian noise.
tau = [-n_max: t_s: n_max]'; phi = [-0.16:0.016:16]';

X = amb(z, t, tau, phi);
mesh(10 * log10(X));
disp('White noise');
xlabel('\tau');
ylabel('\phi');
pause;
