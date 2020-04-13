
% В 2016а не работает...
%M = readmatrix('test.txt')

filename = 'data.txt';
[A,delimiterOut] = importdata(filename);

[X,Y] = size(A);
%z = peaks(25);

figure;
waterfall(1:Y, 1:X, A);
xlabel('Path Length');
ylabel('Fly Count');
zlabel('Time');
%mesh(A)