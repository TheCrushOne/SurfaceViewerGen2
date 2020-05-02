basePath = '../../logs/test_g2/26042020_193042_TRR';
dataPath = strcat(basePath, '.dat');
metaPath = strcat(basePath, '.meta');
[data, delimiterOut] = importdata(dataPath);
jsonDee = jsondecode(fileread(metaPath));
thread_pool_size = size(jsonDee.thread_pool_values, 1);
task_pool_size = size(jsonDee.task_pool_values, 1);
fly_count_size = size(jsonDee.fly_count_values, 1);

toPlot = zeros(thread_pool_size, task_pool_size, fly_count_size);
for rowIdx=1:1:size(data, 1)
    toPlot(data(rowIdx, 1) + 1, data(rowIdx, 2) + 1, data(rowIdx, 3) + 1) = data(rowIdx, 4);
end

figure;
hold on;



for threadIdx=1:1:thread_pool_size
    mtxs = zeros(task_pool_size, fly_count_size);
    for taskIdx=1:1:task_pool_size
        for flyIdx=1:1:fly_count_size
            mtxs(taskIdx, flyIdx) = toPlot(threadIdx, taskIdx, flyIdx);
        end
    end
    mesh(jsonDee.task_pool_values, jsonDee.fly_count_values, mtxs', 'LineWidth', 2);
end
alpha 0.1;
grid on;
xlabel('task pool');
ylabel('fly count');
zlabel('time');