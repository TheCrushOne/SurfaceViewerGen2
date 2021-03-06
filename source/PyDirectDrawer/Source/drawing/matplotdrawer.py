import numpy as np

import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.colors as mcolors
from matplotlib.ticker import (AutoMinorLocator, MultipleLocator)
from drawing.draw_type import DrawType

CELL_HEIGHT = 9

class DrawingProvider():
    fig = {}
    ax_gnt = {}

    draw_type = ''
    
    ax_stairs = None
    ax_gantt = None

    def get_bw_color(self, color):
        if (color[0] + color[1] + color[2] > 1.5):
            return 'black'
        return 'white'

    def plot_single_shard(self, shard_data):
        bar = self.ax_gnt[0].broken_barh(
            [shard_data.interval],
            (shard_data.idx*(CELL_HEIGHT + 1), CELL_HEIGHT),
            facecolors = shard_data.color,
            label = shard_data.prefix + str(shard_data.unit_idx)
        ) 

        center_x = shard_data.interval[0] + shard_data.interval[1]/2
        center_y = shard_data.idx*(CELL_HEIGHT + 1) + CELL_HEIGHT/2

        self.ax_gnt[0].text(
            x = center_x, 
            y = center_y,
            s = "u:" + str(shard_data.unit_idx) + " s:" + str(shard_data.shard_idx), 
            ha = 'center', 
            va = 'center',
            color = self.get_bw_color(mcolors.to_rgba(shard_data.color)),
        )       

    def plot_vlines(self, vline_data):
        for pair in vline_data:
            self.ax_gnt[0].axvline(x=pair.start, color='g')
            self.ax_gnt[0].axvline(x=pair.finish, color='r')

    def plot_shards(self, shards_data):
        for shard in shards_data:
            self.plot_single_shard(shard)
    
    def plot_packet_delimiters(self, del_data):
        pass
        #for line_pair if del_data:
            #self.ax_gnt[0].axvline(line_pair.start)
            #self.ax_gnt[0].text(line_pair.start + 0.1, 0, 'st', rotation=90)
            #self.ax_gnt[0].axvline(line_pair.finish)
            #self.ax_gnt[0].text(line_pair.start + 0.1, 0, 'fn', rotation=90)

    def plot_stairs(self, stairs_data):
        x = np.arange(len(stairs_data))
        y = stairs_data

        plt.step(x, y, label='readiness')
        plt.plot(x, y, 'C0o', alpha=0.5)
        #self.ax_gnt[0].axvline(x=2.20589566)

    def prepare_plot_template(self):
        if (self.draw_type == DrawType.DT_SPLITTED):
            self.fig, self.ax_gnt = plt.subplots(1, 2, figsize = (12, 8))
            self.ax_gantt = self.ax_gnt[0]
            self.ax_stairs = self.ax_gnt[1]
        elif (self.draw_type == DrawType.DT_SMSTAIRS):
            self.fig, self.ax_gnt = plt.subplots(1, 1, figsize = (12, 12))
            self.ax_stairs = self.ax_gnt

    def plot_finisher(self, unitCount, pathLength, packetSize, threadCount):
        if (self.ax_gantt is not None):
            self.ax_gantt.grid(True)
            self.ax_gantt.legend()
            self.ax_gantt.set_ylabel('holder index')
            self.ax_gantt.set_xlabel('time, ms')
        #self.ax_gnt[1].grid(True)
        self.ax_stairs.legend()
        self.ax_stairs.set_ylabel('ready path count')
        self.ax_stairs.set_xlabel('finished packet count')

        # Change major ticks to show every 5.
        self.ax_stairs.xaxis.set_major_locator(MultipleLocator(5))
        self.ax_stairs.yaxis.set_major_locator(MultipleLocator(5))

        # Change minor ticks to show every 1. (5/5 = 1)
        self.ax_stairs.xaxis.set_minor_locator(AutoMinorLocator(5))
        self.ax_stairs.yaxis.set_minor_locator(AutoMinorLocator(5))

        # Turn grid on for both major and minor ticks and style minor slightly
        # differently.
        self.ax_stairs.grid(which='major', color='#CCCCCC', linestyle='--')
        self.ax_stairs.grid(which='minor', color='#CCCCCC', linestyle=':')
        
        self.fig.suptitle('Experiment for: ' + str(unitCount) + ' units, ' + str(pathLength) + ' path length, ' + str(packetSize) + ' tasks packet size, ' + str(threadCount) + ' thread count')
        self.fig.autofmt_xdate() 
        plt.show()

    def draw(self, draw_type, gantt_data, vlines_data, stairs_data):
        assert(len(gantt_data) == len(stairs_data) == len(vlines_data))
        self.draw_type = draw_type
        if (draw_type == DrawType.DT_SPLITTED):
            for i in range(len(gantt_data)):
                self.prepare_plot_template()
                self.plot_shards(gantt_data[i])
                self.plot_vlines(vlines_data[i])
                self.plot_stairs(stairs_data[i])
                self.plot_finisher(0, 0, 0, 0)
        elif (draw_type == DrawType.DT_SMSTAIRS):
            self.prepare_plot_template()
            for i in range(len(gantt_data)):
                #self.prepare_plot_template()
                #self.plot_shards(gantt_data[i])
                #self.plot_vlines(vlines_data[i])
                self.plot_stairs(stairs_data[i])
                #self.plot_finisher(0, 0, 0, 0)
            self.plot_finisher(0, 0, 0, 0)
