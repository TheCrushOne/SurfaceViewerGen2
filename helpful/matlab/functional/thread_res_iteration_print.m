function [] = thread_res_iteration_print(raw_data, view_settings, label_settings, idx_assigner, idx_size, idx_values, outer_idx)
    figure;
    % «ависимость времени расчета от размера пула потоков и количества летающих
    % роботов на разных объемах пула задач
    hold on;
    first_dim_data_size = idx_size(2);
    second_dim_data_size = idx_size(3);
    third_dim_data_size = idx_size(4);
    % idx_values
    second_axle_values = idx_values.third;
    third_axle_values = idx_values.fourth;
    for firstIdx=1:1:first_dim_data_size
        mtxs = zeros(second_dim_data_size, third_dim_data_size);
        for secondIdx=1:1:second_dim_data_size
            for thirdIdx=1:1:third_dim_data_size
                dataIdxVct = [outer_idx, firstIdx, secondIdx, thirdIdx];
                plotIdxVct = zeros(1, 4);
                for i=1:1:4
                    plotIdxVct(idx_assigner(i)) = dataIdxVct(i);
                end
                mtxs(secondIdx, thirdIdx) = raw_data(plotIdxVct(1), plotIdxVct(2), plotIdxVct(3), plotIdxVct(4));
            end
        end
        mesh(second_axle_values, third_axle_values, mtxs', 'LineWidth', 2);
    end
    hold off;
    view(view_settings.azimuth, view_settings.elevation);
    alpha 0.1;
    grid on;
    xlabel(label_settings.xylabels(1));
    ylabel(label_settings.xylabels(2));
    zlabel(label_settings.zlabel);
    title(label_settings.title);
end