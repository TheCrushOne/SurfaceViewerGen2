function [outdata] = gen2rsdrawer(data_1, data_2, data_3, type)
    if (type == 's')
        %waterfall(data.X, data.Y, data.Z.single');
        mesh(data_1.X, data_1.Y, data_1.Z.single');
        hold on;
        mesh(data_2.X, data_2.Y, data_2.Z.single');
        hold on;
        mesh(data_3.X, data_3.Y, data_3.Z.single');
    else
        %waterfall(data.X, data.Y, data.Z.multi');
        mesh(data_1.X, data_1.Y, data_1.Z.multi');
        hold on;
        mesh(data_2.X, data_2.Y, data_2.Z.multi');
        hold on;
        mesh(data_3.X, data_3.Y, data_3.Z.multi');
    end
    box on;
    axis on;    
    
    %title(strcat(data.title, strcat('(', type, ')')));
    %xlabel(data.xlabel);
    %ylabel(data.ylabel);
    %zlabel(data.zlabel);
end

