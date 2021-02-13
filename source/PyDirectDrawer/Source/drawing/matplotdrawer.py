import numpy as np

import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.colors as mcolors

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

        #if self.vlines_cluster.get(shard_data["packet_idx"]) is None:
            #self.vlines_cluster[shard_data["packet_idx"]] = {
                #min = sys.maxint,
                #max = 0
            #}
        #cl_item = self.vlines_cluster.get(shard_data["packet_idx"])
        #if (shard_data["packet_idx"])

    def plot_shards(self, shards_data):
        for shard in shards_data:
            self.plot_single_shard(shard)
    
    def plot_stairs(self, stairs_data):
        x = np.arange(len(stairs_data))
        y = stairs_data

        plt.step(x, y, label='vk')
        plt.plot(x, y, 'C0o', alpha=0.5)

    def prepare_plot_template(self):
        self.fig, self.ax_gnt = plt.subplots(1, 2, figsize = (12, 8))

    def plot_finisher(self, unitCount, pathLength, packetSize, threadCount):
        self.ax_gnt[0].grid(True)
        self.ax_gnt[0].legend()
        
        self.fig.suptitle('Experiment for: ' + str(unitCount) + ' units, ' + str(pathLength) + ' path length, ' + str(packetSize) + ' tasks packet size, ' + str(threadCount) + ' thread count')
        self.fig.autofmt_xdate() 
        plt.show()

    def draw(self, gantt_data, stairs_data):
        assert(len(gantt_data) == len(stairs_data))
        for i in range(len(gantt_data)):
            self.prepare_plot_template()
            self.plot_shards(gantt_data[i])
            self.plot_stairs(stairs_data[i])
            self.plot_finisher(0, 0, 0, 0)