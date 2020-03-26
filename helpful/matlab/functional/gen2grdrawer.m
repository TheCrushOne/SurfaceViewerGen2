function [ ] = gen2grdrawer( rows, cols, idx, data )
%GRDRAWER Summary of this function goes here
%   Detailed explanation goes here
    subplot(rows, cols, idx);
    waterfall(data.X, data.Y, data.Z.single');
    title(strcat(data.title, '(S)'));
    xlabel(data.xlabel);
    ylabel(data.ylabel);
    zlabel(data.zlabel);
    subplot(rows, cols, idx + cols);
    waterfall(data.X, data.Y, data.Z.multi');
    title(strcat(data.title, '(M)'));
    xlabel(data.xlabel);
    ylabel(data.ylabel);
    zlabel(data.zlabel);
end

