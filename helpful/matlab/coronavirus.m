cDRC = [259 457 688 769 1771 1459 1737 1981 2099 2589 2825 3235 3884 3694 3143 3385 2652 2973 2467 2015 14108 5090 2641 2008 2048 1888 1749 391 889 823 648 214 508 406 433 327 427 573 202 125 119 139 143];
cx = 1:1:size(cDRC, 2);
cxx = 1:1:size(cDRC, 2);
cyy = spline(cx, cDRC, cxx);
figure;
plot(cx,cDRC,'o',cxx,cyy);

rDRC = [30 21 33 52 54 53 61 71 57 163 182 196 228 270 302 501 440 771 601 582 658 954 1154 1175 1459 1786 1667 2186 2558 2774 3388 3448 4070 4785 6060];
rx = 1:1:size(rDRC, 2);
rxx = 1:1:size(rDRC, 2);
ryy = spline(rx, rDRC, rxx);
figure;
plot(rx,rDRC,'o',rxx,ryy);

exppt = size(rDRC, 2):1:size(rDRC, 2)*2
expls = linspace(size(rDRC, 2), size(rDRC, 2)*2, 100);
fs = spline(rx,rDRC);

hold on;
plot(expls, ppval(fs, expls),'-', exppt, ppval(fs, exppt), 'o');
grid on;


plot(cxx,cyy,rxx,ryy);
grid on;