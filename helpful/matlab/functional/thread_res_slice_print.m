function [] = thread_res_slice_print(raw_data, view_settings, outer_idx)
    %���� �� ������ ��������, �.�. ����� 3 �������� �� 4 ���� ���
    %�������� 5-��
    idx_assigner = [1, 2, 3, 4];
    label_settings = {};
    label_settings.xylabels = [];
    label_settings.zlabel = 'time';
    label_settings.title = '';
    thread_res_iteration_print(raw_data, view_settings, label_settings, idx_assigner, outer_idx);
    idx_assigner = [1, 4, 2, 3];
    thread_res_iteration_print(raw_data, view_settings, label_settings, idx_assigner, outer_idx); 
    idx_assigner = [1, 3, 4, 2];
    thread_res_iteration_print(raw_data, view_settings, label_settings, idx_assigner, outer_idx);
end