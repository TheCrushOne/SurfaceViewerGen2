basePath = '../../logs/test_g2/09052020_164121_TRR';
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

idx_size = {};
idx_size.thread = thread_pool_size;
idx_size.task = task_pool_size;
idx_size.fly = fly_count_size;
idx_size.length = length_size;
idx_values = {};
idx_values.thread = jsonDee.thread_pool_values;
idx_values.task = jsonDee.task_pool_values;
idx_values.fly = jsonDee.fly_count_values;
idx_values.length = jsonDee.length_values;
idx_titles = ["thread" "task" "fly" "length"];
%idx_titles = ['thread' 'task' 'fly' 'length'];
%test = idx_titles(1) + " = " + 2 + "; " + idx_titles(2) + " = diff";
for thread_idx=1:1:thread_pool_size
    thread_res_slice_print(toPlot, view_settings, idx_size, idx_values, idx_titles, thread_idx);
end
