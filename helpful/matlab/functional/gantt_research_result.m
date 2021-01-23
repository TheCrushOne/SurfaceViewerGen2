function dataset = gantt_research_result(history, plot_idx)
% GanttChart Demo with multiple tasks al? JFreeGraph-Demo
%
% The code behind is just a demo of what is possible with JFreeChart using it in Matlab. I played a little
% with codesnippets I found on the web and the API-Documentation.
% (http://www.jfree.org/jfreechart/api/javadoc/index.html). When  you want to explore the whole functionality,
% I think it is better to buy the JFreeChart Developer Guide (http://www.jfree.org/jfreechart/devguide.html). 
%
% This function shows a single domain multiple range axis plot as an example of JFreeChart
% (http://www.jfree.org/). The Idea to this code is based on the UndocumentedMatlab-Blog of Yair Altman, who
% shows a sample Code of JFreeChart for creating a PieChart
% (http://undocumentedmatlab.com/blog/jfreechart-graphs-and-gauges/#comments) 
%
% Within the plot you can zoom by pressing the left mouse button and moving the pointer. Also you have some
% properties by right-clicking on the chart. With the slider or by mousclick in the chart you can set the
% position of the crosshair. The actual values of the crosshair are displayed in the table.  
%
% Before this demo works, you need to download JFreeChart and make matlab get to know with it. There are 2
% ways you can do this:
% 1. Add the jcommon and jfreechart jar to the dynamic matlab JavaClassPath (uncommented lines in the first
%    cell an change path to your local installation path)
% 2. Add the jcommon and jfreechart jar to the static matlab JavaClassPath (see Matlab Help, modify
%    classpath.txt on matlabroot\toolbox\local) 
%
% Finally you must donwload jcontrol from Malcom Lidierth
% (http://www.mathworks.com/matlabcentral/fileexchange/15580-using-java-swing-components-in-matlab).
% 
%
% Bugs and suggestions:
%    Please send to Sven Koerner: koerner(underline)sven(add)gmx.de
% 
% You need to download and install first:
%    http://sourceforge.net/projects/jfreechart/files/1.%20JFreeChart/1.0.13/ 
%    http://sourceforge.net/projects/jfreechart/files/1.%20JFreeChart/1.0.9/
%    http://www.mathworks.com/matlabcentral/fileexchange/15580-using-java-swing-components-in-matlab 
%
% Programmed by Sven Koerner: koerner(underline)sven(add)gmx.de
% Date: 2011/05/25 
%%  JFreeChart to matlab
%%%  Add the JavaPackages to the static javaclasspath (see Matlab Help, modify classpath.txt on
%%%  matlabroot\toolbox\local) or alternativ turn it to the dynamic path (uncomment the next and change path to jFreeeChart) 
% javaaddpath C:/Users/sk/Documents/MATLAB/jfreechart-1.0.13/lib/jcommon-1.0.16.jar
% javaaddpath C:/Users/sk/Documents/MATLAB/jfreechart-1.0.13/lib/jfreechart-1.0.13.jar


%% Start
%import java.awt.Color;
%import jfreechart-1.0.19.StandardLegend;
% create_IntervalCategoryDataset 
dataset = create_IntervalCategoryDataset(history);
% generate chart 
chart = org.jfree.chart.ChartFactory.createGanttChart('Gantt Chart Demo ' + string(plot_idx), 'Task', 'Date', dataset, true, true, false);
chart.getCategoryPlot().getDomainAxis().setMaximumCategoryLabelWidthRatio(10);
%chart.getCategoryPlot().getRenderer().setLabelGenerator(new CustomCategoryLabelGenerator("{2}", new DecimalFormat("0.00")));
%StandardLegend legend = (StandardLegend) chart.getLegend();
%legend.setDisplaySeriesShapes(true);
% generate Panel
chartPanel = org.jfree.chart.ChartPanel(chart);
chartPanel.setPreferredSize(java.awt.Dimension(500, 270));
chartPanel.setVisible(true);
%% Show the GanttChart
jPanel2 = chartPanel;                                                     % create new panel
fh      = figure('Units','normalized','position',[0.1,0.1,  0.7,  0.7]);  % create new figure
jp      = jcontrol(fh, jPanel2,'Position',[0.01 0.01 0.98 0.98]);         % add the jPanel to figure
function collection = create_IntervalCategoryDataset(history)
%% Function for IntervalCategory-Dataset Generation
% Create first TaskSeries
holder_run_data = history.cluster_run_data.holder_run_data;
holder_count = holder_run_data.count;
unit_count = history.cluster_run_data.unit_count;
max_task_count = 0;
for holderIdx=0:1:holder_count-1
    tasklist = holder_run_data.(char("x" + string(holderIdx)));
    task_count = size(tasklist, 1);
    if task_count > max_task_count
        max_task_count = task_count
    end
end

%series_list = 
for seriesIdx=1:1:unit_count
    series_list(seriesIdx) = org.jfree.data.gantt.TaskSeries('Unit ' + string(seriesIdx))
end
% TimePeriods
time_periods = []
%series_list
%for taskIdx=1:1:max_task_count
%    series_list(taskIdx) = org.jfree.data.gantt.TaskSeries(string(taskIdx));
%end
for holderIdx=0:1:holder_count-1
    tasklist = holder_run_data.(char("x" + string(holderIdx)));
    task_count = size(tasklist, 1);
    for taskIdx=1:1:task_count
        elem = tasklist(taskIdx);
        time_period = org.jfree.data.time.SimpleTimePeriod(elem.start_ts, elem.finish_ts);
        series_list(elem.unit_idx + 1).add(org.jfree.data.gantt.Task(string(holderIdx), time_period));
    end
    %for taskIdx=task_count:1:max_task_count
%        elem = tasklist(taskIdx);
%        time_period = org.jfree.data.time.SimpleTimePeriod(0, 0);
%        series_list(elem.unit_idx + 1).add(org.jfree.data.gantt.Task(string(holderIdx), time_period));
%    end
end
% SeriesCollection
collection      =  org.jfree.data.gantt.TaskSeriesCollection();
for taskIdx=1:1:unit_count
    collection.add(series_list(taskIdx))
end
%collection.add(so);
%collection.add(s1);
%collection.add(s2);
