function [outdata] = gen1rsdrawer(data, type)
    width = 5;
    if (type == 's')
        %waterfall(data.X, data.Y, data.Z.single');
        mesh(data.X, data.Y, data.Z.single', 'LineWidth', width);
    else
        %waterfall(data.X, data.Y, data.Z.multi');
        mesh(data.X, data.Y, data.Z.multi', 'LineWidth', width);
    end
    box on;
    axis on;
    %title(strcat(data.title, strcat('(', type, ')')));
    %title(strcat(, strcat('(', type, ')')));
    xlabel('Количество дронов');%data.xlabel);
    ylabel('Длина Пути');%data.ylabel);
    zlabel('Время');%data.zlabel);
    axis([10 50 1000 5000 0 150000]);
end

