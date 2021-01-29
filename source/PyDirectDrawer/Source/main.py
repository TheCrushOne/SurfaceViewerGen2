import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.colors as mcolors
import pandas as pd
import numpy as np
import json
import sys
import os
import glob
#from adjustText import adjust_text

DATA_FILE_PATH = "E:\\FilesX\\PrOgRaMmInG\\2019_2020_9th_course\\SurfaceViewerGen2\\cache\\gantt_demo\\research\\29012021_012018_universal.rres"
DATA_FOLDER_PATH = "..\\..\\cache\\gantt_demo\\research\\"

CELL_HEIGHT = 9

class Plotter:
    fig = {}
    ax_gnt = {}
    label_pool = []
    color_palette = []
    
    def get_bw_color(self, color):
        if (color[0] + color[1] + color[2] > 1.5):
            return 'black'
        return 'white'

    def allocate_color_palette(self, count):
        color_names = list(mcolors.CSS4_COLORS)
        return color_names

    def prepare_plot_template(self):
        self.fig, self.ax_gnt = plt.subplots(figsize = (12, 8))

    def __init__(self):
        pass
        #self.service = self.Service()

    def plot_finisher(self, unitCount, pathLength, packetSize, threadCount):
        self.ax_gnt.grid(True)
        self.ax_gnt.legend()
        
        self.fig.suptitle('Experiment for: ' + str(unitCount) + ' units, ' + str(pathLength) + ' path length, ' + str(packetSize) + ' tasks packet size, ' + str(threadCount) + ' thread count')
        self.fig.autofmt_xdate() 
        plt.show()

    def plot_single_shard(self, shard_data):
        if shard_data["unit_idx"] in self.label_pool:
            prefix = '_'
        else:
            self.label_pool.append(shard_data["unit_idx"])
            prefix = ''
        interval = (shard_data["start_ts"], shard_data["finish_ts"] - shard_data["start_ts"])
        color = self.color_palette[shard_data["unit_idx"]]
        idx = shard_data["holder_idx"]
        bar = self.ax_gnt.broken_barh(
            [interval],
            (idx*(CELL_HEIGHT + 1), CELL_HEIGHT),
            facecolors = color,
            label = prefix + str(shard_data["unit_idx"])
        ) 

        center_x = interval[0] + interval[1]/2
        center_y = idx*(CELL_HEIGHT + 1) + CELL_HEIGHT/2

        self.ax_gnt.text(
            x = center_x, 
            y = center_y,
            s = "u:" + str(shard_data["unit_idx"]) + " s:" + str(shard_data["shard_idx"]), 
            ha = 'center', 
            va = 'center',
            color = self.get_bw_color(mcolors.to_rgba(color)),
        )

    def parse_holder_run_data(self, hrd):
        for shard in hrd["data"]:
            stat_unit = shard["statistic_unit"]
            self.plot_single_shard(stat_unit)

    def parse_holder_cluster_run_data(self, hcrd):
        for shard in hcrd["data"]: 
            self.parse_holder_run_data(shard["holder_run_data"])

    def parse_holder_cluster_run_history(self, hcrh):
        for shard in hcrh["history"]:
            self.color_palette = self.allocate_color_palette(hcrh["unit_count"])
            self.prepare_plot_template() 
            self.parse_holder_cluster_run_data(shard["holder_cluster_run_data"])
            self.plot_finisher(hcrh["unit_count"], 0, 0, 0)

    def parse_experiment_history(self, exh):
        for shard in exh["history"]:
            self.parse_holder_cluster_run_history(shard["holder_cluster_run_history"])
            
    def run(self, data_file_path):
        with open(data_file_path) as json_file:
            data = json.load(json_file)
            self.parse_experiment_history(data["experiment_history"])
            
    def get_latest_file(self, path):
        list_of_files = glob.glob(os.path.join(path, "*.rres"))
        latest_file = max(list_of_files, key=os.path.getctime)
        return latest_file

if __name__ == '__main__':
    plotter = Plotter()
    plotter.run(sys.argv[1] if len(sys.argv) > 1 else plotter.get_latest_file(DATA_FOLDER_PATH))