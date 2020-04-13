datadir = '../17082019_1915/';

data = cvparser(datadir);

figure;
waterfall(data.X, data.Y, data.Z');
hold on;
clr = zeros(size(data.Z));
[m,n] = size(clr);
Z = imread('../../maps/test_low_new.png');
for row=1:1:m
    for col=1:1:n
        if (data.Z(row, col) == 1); clr(row, col) =  Z(row, col); else; clr(row, col) = NaN; end;
    end
    
end
surf(Z, clr, 'FaceAlpha', 1.0, 'EdgeColor', 'none');
%colorbar;