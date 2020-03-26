basePath = '../../logs/test_g2/27032020_001748_TRR';
dataPath = strcat(basePath, '.dat');
metaPath = strcat(basePath, '.meta');
[data, delimiterOut] = importdata(dataPath);
jsonDee = jsondecode(fileread(metaPath));
toPlot = zeros(jsonDee.thread_pool_summary, jsonDee.task_pool_summary, jsonDee.fly_count_summary);
for rowIdx=1:1:size(data, 1)
    toPlot(data(rowIdx, 1) + 1, data(rowIdx, 2) + 1, data(rowIdx, 3) + 1) = data(rowIdx, 4);
end

figure;
hold on;
for rowIdx=1:1:data(jsonDee.thread_pool_summary)
    %waterfall(1:jsonDee.task_pool_summary, 1:jsonDee.fly_count_summary, squeeze(toPlot(rowIdx, :, :)));
    mesh(1:jsonDee.task_pool_summary, 1:jsonDee.fly_count_summary, squeeze(toPlot(rowIdx, :, :)), 'LineWidth', 1);
end

xlabel('task_pool_summary');
ylabel('fly_count_summary');
zlabel('Time');