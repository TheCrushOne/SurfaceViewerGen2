function [] = thread_res_slice_print(raw_data, view_settings, idx_size, idx_values, idx_titles, outer_idx)
    %срез по одному значению, т.е. вывод 3 графиков по 4 осям для
    %значений 5-ой
    label_settings = {};
    label_settings.zlabel = 'time';
    
    idx_values_local = {};
    
    idx_assigner = [1, 2, 3, 4];
    label_settings.xylabels = [idx_titles(3) idx_titles(4)];
    label_settings.title = idx_titles(1) + " = " + idx_values.thread(outer_idx) + "; " + idx_titles(2) + " = diff";
    idx_size_local = [idx_size.thread, idx_size.task, idx_size.fly, idx_size.length];
    idx_values_local.first = idx_values.thread;
    idx_values_local.second = idx_values.task;
    idx_values_local.third = idx_values.fly;
    idx_values_local.fourth = idx_values.length;
    thread_res_iteration_print(raw_data, view_settings, label_settings, idx_assigner, idx_size_local, idx_values_local, outer_idx);
    
    idx_assigner = [1, 4, 2, 3];
    label_settings.xylabels = [idx_titles(2) idx_titles(3)];
    label_settings.title = idx_titles(1) + " = " + idx_values.thread(outer_idx) + "; " + idx_titles(4) + " = diff";
    idx_size_local = [idx_size.thread, idx_size.length, idx_size.task, idx_size.fly];
    idx_values_local.first = idx_values.thread;
    idx_values_local.second = idx_values.length;
    idx_values_local.third = idx_values.task;
    idx_values_local.fourth = idx_values.fly;
    thread_res_iteration_print(raw_data, view_settings, label_settings, idx_assigner, idx_size_local, idx_values_local, outer_idx); 
    
    idx_assigner = [1, 3, 4, 2];
    label_settings.xylabels = [idx_titles(4) idx_titles(2)];
    label_settings.title = idx_titles(1) + " = " + idx_values.thread(outer_idx) + "; " + idx_titles(3) + " = diff";
    idx_size_local = [idx_size.thread, idx_size.fly, idx_size.length, idx_size.task];
    idx_values_local.first = idx_values.thread;
    idx_values_local.second = idx_values.fly;
    idx_values_local.third = idx_values.length;
    idx_values_local.fourth = idx_values.task;
    thread_res_iteration_print(raw_data, view_settings, label_settings, idx_assigner, idx_size_local, idx_values_local, outer_idx);
end