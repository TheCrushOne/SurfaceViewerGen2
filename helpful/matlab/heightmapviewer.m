data = struct;
[data, delimiterOut] = importdata('test_g2.dat');
xAxis = 1:1:size(data, 1);
yAxis = 1:1:size(data, 2);
waterfall(xAxis, yAxis', data');
xlabel('x');
ylabel('y');
dmax = max(data, [], 'all');
dmin = min(data, [], 'all');