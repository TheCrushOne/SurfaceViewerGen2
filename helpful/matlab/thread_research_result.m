basePath = '../../logs/test_g2/02052020_160857_TRR';
dataPath = strcat(basePath, '.dat');
metaPath = strcat(basePath, '.meta');
[data, delimiterOut] = importdata(dataPath);
jsonDee = jsondecode(fileread(metaPath));
thread_pool_size = size(jsonDee.thread_pool_values, 1);
task_pool_size = size(jsonDee.task_pool_values, 1);
fly_count_size = size(jsonDee.fly_count_values, 1);
length_size = size(jsonDee.length_values, 1);

toPlot = zeros(thread_pool_size, task_pool_size, fly_count_size, length_size);
for rowIdx=1:1:size(data, 1)
    toPlot(data(rowIdx, 1) + 1, data(rowIdx, 2) + 1, data(rowIdx, 3) + 1, data(rowIdx, 4) + 1) = data(rowIdx, 5);
end

%figure;
%hold on;

view_settings = {};

view_settings.azimuth = -45;
view_settings.elevation = 45;

thread_res_slice_print(view_settings);

% «ависимость времени расчета от размера пула задач и количества летающих
% роботов на разных объемах пула потоков
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
hold off;
view(viewAzimuth, viewElevation);
alpha 0.1;
grid on;
xlabel('task pool');
ylabel('fly count');
zlabel('time');
title('Time on several thread pool values');


figure;
% «ависимость времени расчета от размера пула потоков и количества летающих
% роботов на разных объемах пула задач
hold on;
for taskIdx=1:1:task_pool_size
    mtxs = zeros(fly_count_size, thread_pool_size);
    for flyIdx=1:1:fly_count_size
        for threadIdx=1:1:thread_pool_size        
            mtxs(flyIdx, threadIdx) = toPlot(threadIdx, taskIdx, flyIdx);
        end
    end
    mesh(jsonDee.fly_count_values, jsonDee.thread_pool_values, mtxs', 'LineWidth', 2);
end
hold off;
view(viewAzimuth, viewElevation);
alpha 0.1;
grid on;
xlabel('fly count');
ylabel('thread pool');
zlabel('time');
title('Time on several task pool values');


figure;
% «ависимость времени расчета от размера пула потоков и количества летающих
% роботов на разных объемах пула задач
hold on;
for flyIdx=1:1:fly_count_size
    mtxs = zeros(thread_pool_size, task_pool_size);
    for threadIdx=1:1:thread_pool_size
        for taskIdx=1:1:task_pool_size
            mtxs(threadIdx, taskIdx) = toPlot(threadIdx, taskIdx, flyIdx);
        end
    end
    mesh(jsonDee.thread_pool_values, jsonDee.task_pool_values, mtxs', 'LineWidth', 2);
end
hold off;
view(viewAzimuth, viewElevation);
alpha 0.1;
grid on;
xlabel('thread pool');
ylabel('task pool');
zlabel('time');
title('Time on several fly count values');

