basePath = '../../logs/test_g2/28032020_021955_TRR';
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

for threadIdx=1:1:jsonDee.thread_pool_summary
    %waterfall(1:jsonDee.task_pool_summary, 1:jsonDee.fly_count_summary, squeeze(toPlot(rowIdx, :, :)));
    %permute(toPlot(rowIdx, :, :), [])
    %tmp = reshape(toPlot(rowIdx, :, :), [], size(toPlot,2), 1);
    mtxs = zeros(jsonDee.task_pool_summary, jsonDee.fly_count_summary);
    for taskIdx=1:1:jsonDee.task_pool_summary
        for flyIdx=1:1:jsonDee.fly_count_summary
            mtxs(taskIdx, flyIdx) = toPlot(threadIdx, taskIdx, flyIdx);
        end
    end
    mesh(1:jsonDee.task_pool_summary, 1:jsonDee.fly_count_summary, mtxs, 'LineWidth', 1);
end
alpha 0.1;
grid on;
xlabel('task pool summary');
ylabel('fly count summary');
zlabel('time');