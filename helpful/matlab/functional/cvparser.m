function [ data ] = cvparser( datadir )
    data = struct;
    fileext = char('matrix.mtxdmp');
    [data.Z, delimiterOut] = importdata(strcat(datadir, fileext));
    [m,n] = size(data.Z);
    data.X = 1:1:m;
    data.Y = 1:1:n;
    data.Z = logical(data.Z);
end

