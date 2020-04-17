data = struct;
[data, delimiterOut] = importdata('E:\FilesX\PrOgRaMmInG\2019-2020(9th_course)\SurfaceViewerGen2\scenarios\test_small\test_small.dat');
xAxis = 1:1:size(data, 1);
yAxis = 1:1:size(data, 2);
waterfall(xAxis, yAxis', data');
xlabel('x');
ylabel('y');
dmax = max(data, [], 'all');
dmin = min(data, [], 'all');