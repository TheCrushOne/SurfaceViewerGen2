import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Insets;

import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.ThermometerPlot;
import org.jfree.data.general.DefaultValueDataset;
import org.jfree.ui.ApplicationFrame;

% First Value of Thermometer
dataset = DefaultValueDataset((43.0));
% Generate plot object
plot  = ThermometerPlot(dataset);
% Generate chart object
chart = JFreeChart('Thermometer Demo', JFreeChart.DEFAULT_TITLE_FONT, plot, false);

% Some optional Customization of the Chart
plot.setInsets(org.jfree.ui.RectangleInsets(5, 5, 5, 5));
plot.setThermometerStroke(java.awt.BasicStroke(2.0));
plot.setThermometerPaint(Color.lightGray);
plot.setSubrangeInfo(ThermometerPlot.NORMAL, 0.0, 55.0, 0.0, 100.0);              % from 0 to 55 Normal
plot.setSubrangeInfo(ThermometerPlot.WARNING, 55.0, 75.0, 0.0, 100.0);            % from 55 to 75 Warning
plot.setSubrangeInfo(ThermometerPlot.CRITICAL, 75.0, 100.0, 0.0, 100.0);          % from >75 to 100 Critical

% generate Chart Panel
cp = ChartPanel(chart);

% New figure
fh = figure('Units','normalized','position',[0.1,0.1,  0.2,  0.4]);

% ChartPanel with JControl
jp = jcontrol(fh, cp,'Position',[0.01 0.07 0.98 0.88]);

% Matlab-Slider
sh = uicontrol(fh,'Style','slider',...
                'Max',100,'Min',0,'Value',43,...
                'SliderStep',[0.01 0.01],...
                'Units','normalized', ...
                'Position',[0.01 0.01 0.98 0.05], ...
                'UserData', {plot}, ...                       % save the handle of the plot-object to Userdata to change values
                'Callback',@sh_callback2);
            
       
