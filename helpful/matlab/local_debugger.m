coord = [52 52 1];
angle = deg2rad(-45);
mv1 = [1 0 0; 0 1 0; -62 -62 1];
rtm = [cos(angle) sin(angle) 0; -sin(angle) cos(angle) 0; 0 0 1];
mv2 = [1 0 0; 0 1 0; 62 62 1];

coord*mv1*rtm*mv2

-30:1:30

xdr = -10:1:10
ydr = -10:1:10

sgm = 1
mu = 0

grad_delta = 30;
fps = 60;
period = 10; %sec
time = 0:0.01:16;
frames = time.*60;
X = pi*cos(frames/(period*2*60)*(2*pi));
tmp = frames/(period*2*60)*(2*pi)
Y = grad_delta*sin(pi*cos(frames/(period*2*60)*(2*pi)));
y = 1/(sgm*sqrt(2*pi))*exp(-0.5*((X - mu)/sgm).^2);
subplot(2, 1, 1)
plot(time, tmp);
grid on
subplot(2, 1, 2)
plot(time, Y);
grid on
