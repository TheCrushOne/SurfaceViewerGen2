function [ data ] = gen2grparser( datadir, type, stratname )
%GRPARSER Summary of this function goes here
%   Detailed explanation goes here
    data = struct;
    s_fileext = char('_s.dmpdat');
    m_fileext = char('_m.dmpdat');
    fileNameTemplate = 'average_fly_length_';
    type = 3;%str2double(metadata.type.Text);
    %xaxis_range = metadata.xaxis.range;
    %data.X = str2double(xaxis_range.min.Text) : str2double(xaxis_range.step.Text) : str2double(xaxis_range.max.Text);
    %yaxis_range = metadata.yaxis.range;
    %data.Y = str2double(yaxis_range.min.Text) : str2double(yaxis_range.step.Text) : str2double(yaxis_range.max.Text);
    
    %strcat(datadir, metadata.datafile_template.Text, s_fileext)
    data.Z = struct;
    if (type == 0)  % ��� ������
    elseif (type == 1)  % ������ �����
        [data.Z.single, delimiterOut] = importdata(strcat(datadir, fileNameTemplate, stratname, s_fileext));
    elseif (type == 2)  % ������ ������
        [data.Z.multi, delimiterOut] = importdata(strcat(datadir, fileNameTemplate, stratname, m_fileext));
    elseif (type == 3)  % ���
        [data.Z.single, delimiterOut] = importdata(strcat(datadir, fileNameTemplate, stratname, s_fileext));
        [data.Z.multi, delimiterOut] = importdata(strcat(datadir, fileNameTemplate, stratname, m_fileext));
    else
    end
    
    [m, n] = size(data.Z.multi);
    data.X = 1:1:m;
    data.Y = 1:1:n;
    %data.xlabel = metadata.xaxis.label.Text;
    %data.ylabel = metadata.yaxis.label.Text;
    %data.zlabel = metadata.zaxis.label.Text;
    %data.title = metadata.title.Text;
end

