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

    fig_s = {}
    fig_t = {}
    ax_gnt_s = {}
    ax_gnt_t = {}

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

    def plot_stairs(self, stairs_data, gnt):
        #x = np.arange(len(stairs_data))
        #y = stairs_data
        x = stairs_data.x
        y = stairs_data.y

        gnt.step(x, y, 
            label=stairs_data.label + ' ' +
            'u:' + str(stairs_data.unit_count) + ' ' +
            'p:' + str(stairs_data.packet_size) + ' ' +
            't:' + str(stairs_data.thread_count) + ' ' +
            's:' + str(stairs_data.split_factor)
        )
        gnt.plot(x, y, 'C0o', alpha=0.5)
        #plt.step(x, y, label='readiness')
        #plt.plot(x, y, 'C0o', alpha=0.5)

        #self.ax_gnt[0].axvline(x=2.20589566)

    def prepare_plot_template(self):
        if (self.draw_type == DrawType.DT_SPLITTED):
            self.fig, self.ax_gnt = plt.subplots(1, 2, figsize = (12, 8))
            self.ax_gantt = self.ax_gnt[0]
            self.ax_stairs = self.ax_gnt[1]
        elif (self.draw_type == DrawType.DT_SMSTAIRS):
            self.fig, self.ax_gnt = plt.subplots(1, 2, figsize = (12, 12))
            self.ax_stairs = self.ax_gnt[0]
            self.ax_stairs_time = self.ax_gnt[1]
        elif (self.draw_type == DrawType.DT_AFTERWARDS):
            self.fig_s = plt.figure()
            self.fig_t = plt.figure()
            self.ax_gnt_s = self.fig_s.add_subplot(111)
            self.ax_gnt_t = self.fig_t.add_subplot(111)
            self.ax_stairs = self.ax_gnt_s
            self.ax_stairs_time = self.ax_gnt_t

    def stairs_finisher(self, ax, factor_x, limiter_x, factor_y, limiter_y, ax_title_x, ax_title_y):
        #self.ax_gnt[1].grid(True)
        ax.legend()
        ax.set_xlabel(ax_title_x)
        ax.set_ylabel(ax_title_y)        

        # Change major ticks to show every factor.
        ax.xaxis.set_major_locator(MultipleLocator(factor_x))
        ax.yaxis.set_major_locator(MultipleLocator(factor_y))

        # Change minor ticks to show every 1. (factor/limiter = x)
        ax.xaxis.set_minor_locator(AutoMinorLocator(limiter_x))
        ax.yaxis.set_minor_locator(AutoMinorLocator(limiter_y))

        # Turn grid on for both major and minor ticks and style minor slightly
        # differently.
        ax.grid(which='major', color='#CCCCCC', linestyle='--')
        ax.grid(which='minor', color='#CCCCCC', linestyle=':')

    def plot_finisher(self, unitCount, pathLength, packetSize, threadCount):
        if (self.ax_gantt is not None):
            self.ax_gantt.grid(True)
            self.ax_gantt.legend()
            self.ax_gantt.set_ylabel('holder index')
            self.ax_gantt.set_xlabel('time, ms')

        self.stairs_finisher(self.ax_stairs, 5, 5, 5, 5, "finished packet count", "ready path count")
        self.stairs_finisher(self.ax_stairs_time, 1000, 5, 5, 5, "computing time, ms", "ready path count")
        
        if (self.draw_type == DrawType.DT_AFTERWARDS):
            #self.ax_stairs.set_title('Experiment for: ' + str(unitCount) + ' units, ' + str(pathLength) + ' path length, ' + str(packetSize) + ' tasks packet size, ' + str(threadCount) + ' thread count')
            self.fig_s.autofmt_xdate() 
            #self.ax_stairs_time.set_title('Experiment for: ' + str(unitCount) + ' units, ' + str(pathLength) + ' path length, ' + str(packetSize) + ' tasks packet size, ' + str(threadCount) + ' thread count')
            self.fig_t.autofmt_xdate() 
        else:
            #self.fig.suptitle('Experiment for: ' + str(unitCount) + ' units, ' + str(pathLength) + ' path length, ' + str(packetSize) + ' tasks packet size, ' + str(threadCount) + ' thread count')
            self.fig.autofmt_xdate() 
        plt.show()

    def draw(self, draw_type, gantt_data, vlines_data, stairs_data, timed_data):
        assert(len(gantt_data) == len(stairs_data) == len(vlines_data) == len(timed_data))
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
                self.plot_stairs(stairs_data[i], self.ax_stairs)
                self.plot_stairs(timed_data[i], self.ax_stairs_time)
                #self.plot_finisher(0, 0, 0, 0)
            self.plot_finisher(0, 0, 0, 0)
        elif (draw_type == DrawType.DT_AFTERWARDS):
            self.prepare_plot_template()
            for i in range(len(gantt_data)):
                self.plot_stairs(stairs_data[i], self.ax_stairs)
                self.plot_stairs(timed_data[i], self.ax_stairs_time)
            self.plot_finisher(0, 0, 0, 0)
