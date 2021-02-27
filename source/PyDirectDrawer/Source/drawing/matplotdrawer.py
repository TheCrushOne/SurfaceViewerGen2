import numpy as np

import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.colors as mcolors
from matplotlib.ticker import (AutoMinorLocator, MultipleLocator)

CELL_HEIGHT = 9

class DrawingProvider():
    fig = {}
    ax_gnt = {}

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
        self.fig, self.ax_gnt = plt.subplots(1, 2, figsize = (12, 8))

    def plot_finisher(self, unitCount, pathLength, packetSize, threadCount):
        self.ax_gnt[0].grid(True)
        self.ax_gnt[0].legend()
        self.ax_gnt[0].set_ylabel('holder index')
        self.ax_gnt[0].set_xlabel('time, ms')
        #self.ax_gnt[1].grid(True)
        self.ax_gnt[1].legend()
        self.ax_gnt[1].set_ylabel('ready path count')
        self.ax_gnt[1].set_xlabel('finished packet count')
        
        self.ax_gnt[1].legend()
        self.ax_gnt[1].legend()

        # Change major ticks to show every 5.
        self.ax_gnt[1].xaxis.set_major_locator(MultipleLocator(5))
        self.ax_gnt[1].yaxis.set_major_locator(MultipleLocator(5))

        # Change minor ticks to show every 1. (5/5 = 1)
        self.ax_gnt[1].xaxis.set_minor_locator(AutoMinorLocator(5))
        self.ax_gnt[1].yaxis.set_minor_locator(AutoMinorLocator(5))

        # Turn grid on for both major and minor ticks and style minor slightly
        # differently.
        self.ax_gnt[1].grid(which='major', color='#CCCCCC', linestyle='--')
        self.ax_gnt[1].grid(which='minor', color='#CCCCCC', linestyle=':')
        
        self.fig.suptitle('Experiment for: ' + str(unitCount) + ' units, ' + str(pathLength) + ' path length, ' + str(packetSize) + ' tasks packet size, ' + str(threadCount) + ' thread count')
        self.fig.autofmt_xdate() 
        plt.show()

    def draw(self, gantt_data, vlines_data, stairs_data):
        assert(len(gantt_data) == len(stairs_data) == len(vlines_data))
        for i in range(len(gantt_data)):
            self.prepare_plot_template()
            self.plot_shards(gantt_data[i])
            self.plot_vlines(vlines_data[i])
            self.plot_stairs(stairs_data[i])
            self.plot_finisher(0, 0, 0, 0)