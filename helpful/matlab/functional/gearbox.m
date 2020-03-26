x = 0:1:17;
y = [27.00 18.00 11.72 9.65 7.916 6.52 5.36 4.41 3.636 2.99 2.58 2.14 1.77 1.464 1.211 1.00 0.83 0.69];
xx = 0:1:17;
yy = spline(x,y,xx);
exppt = [18 19 20 21]
expls = linspace(18, 21, 100);
fs = spline(x,y);
plot(x,y,'o',xx,yy);
hold on;
plot(expls, ppval(fs, expls),'-', exppt, ppval(fs, exppt), 'o');
grid on;