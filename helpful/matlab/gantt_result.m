javaaddpath jfreecharts/jcommon-1.0.23.jar
javaaddpath jfreecharts/jfreechart-1.0.19.jar
javaaddpath jfreecharts/jfreechart-1.0.19-experimental.jar            
javaaddpath jfreecharts/jfreechart-1.0.19-swt.jar  

resDataPath = '../../cache/gantt_demo/research/universal.rres';
jsonDee = jsondecode(fileread(resDataPath));
experiment_history = jsonDee.experiment_history;
cluster_history = experiment_history.cluster_history;
[history_count, dd] = size(cluster_history);

historyIdx = 1
%for historyIdx=1:1:history_count
    history_shard = cluster_history(historyIdx);
    [holder_count, task_count] = size(history_shard);   
    gantt_research_result(history_shard, historyIdx);
%end

%cdr = history_shard.cluster_run_data.holder_run_data
    %addr = 'x' + string(1)
    %cnt1 = char("count")
    %cnt2 = 'count'
    %cdr.(addr)

%time_data = []
%max_task_count = 0
%for holderIdx=1:1:holder_count
    %time_data(holderIdx, 1) = 0
    %max_task_count = max(max_task_count, taskCount)
    %for taskIdx=1:1:taskCount
        %time_data(holderIdx, 2*taskIdx) = history(holderIdx, taskIdx).start_ts;
        %time_data(holderIdx, 2*taskIdx+1) = history(holderIdx, taskIdx).finish_ts;
    %end
%end

%Positions=1:1:holder_count;
%Positions = [1 2 3 4]
%Gap_Duration=[0,2,5,3,5,3;
              %3,5,3,5,3,4;
              %9,3,0,0,12,2;
              %13,2,2,2,8,3];
%H = barh(Positions,Gap_Duration,'stacked')
%H = barh(Positions,time_data,'stacked')
%rv = 2:2:2*max_task_count(1)+1
%set(H(rv),'Visible','off')