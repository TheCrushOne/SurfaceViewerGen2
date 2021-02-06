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

class LogLevelToken():
   LLT_OVERALL    = 'overall_log'
   LLT_PIPELINE   = 'pipeline_run_log'
   LLT_RUN        = 'run_log'
   LLT_PACKET     = 'packet_log' 
   LLT_TASK       = 'task_log'

   LLT_SUB_HISTORY = 'history'
   LLT_SUB_DATA    = 'data'

class LogValueToken():
    LVT_START_TS    = 'start_ts'
    LVT_FINISH_TS   = 'finish_ts'
    LVT_HOLDER_IDX  = 'holder_idx'
    LVT_PACKET_IDX  = 'packet_idx'
    LVT_SHARD_IDX   = 'shard_idx'
    LVT_TASK_IDX    = 'task_idx'
    LVT_UNIT_IDX    = 'unit_idx'

    LVT_UNIT_COUNT  = 'unit_count'

class Plotter:
    fig = {}
    ax_gnt = {}
    label_pool = []
    color_palette = []
    vlines_cluster = dict()
    
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

        #if self.vlines_cluster.get(shard_data["packet_idx"]) is None:
            #self.vlines_cluster[shard_data["packet_idx"]] = {
                #min = sys.maxint,
                #max = 0
            #}
        #cl_item = self.vlines_cluster.get(shard_data["packet_idx"])
        #if (shard_data["packet_idx"])

    def parse_task_log(self, hrd):
        #for shard in hrd[LogLevelToken.LLT_SUB_DATA]:
            #stat_unit = shard[LogLevelToken.LLT_TASK]
            self.plot_single_shard(hrd)

    def parse_packet_log(self, hcrd):
        for shard in hcrd[LogLevelToken.LLT_SUB_DATA]: 
            self.parse_task_log(shard[LogLevelToken.LLT_TASK])

    #def draw_vlines(self, hcrd):
        #self.vlines_cluster

    def parse_run_log(self, hcrh):
        for shard in hcrh[LogLevelToken.LLT_SUB_DATA]:
            self.color_palette = self.allocate_color_palette(hcrh[LogValueToken.LVT_UNIT_COUNT])
            self.prepare_plot_template() 
            self.parse_packet_log(shard[LogLevelToken.LLT_PACKET])
            #self.draw_vlines()
            self.plot_finisher(hcrh[LogValueToken.LVT_UNIT_COUNT], 0, 0, 0)

    def parse_pipeline_log(self, exh):
        for shard in exh[LogLevelToken.LLT_SUB_HISTORY]:
            self.parse_run_log(shard[LogLevelToken.LLT_RUN])

    def parse_overall_log(self, exh):
        for shard in exh[LogLevelToken.LLT_SUB_HISTORY]:
            self.parse_pipeline_log(shard[LogLevelToken.LLT_PIPELINE])
            
    def run(self, data_file_path):
        with open(data_file_path) as json_file:
            data = json.load(json_file)
            self.parse_overall_log(data[LogLevelToken.LLT_OVERALL])
            
    def get_latest_file(self, path):
        list_of_files = glob.glob(os.path.join(path, "*.rres"))
        latest_file = max(list_of_files, key=os.path.getctime)
        return latest_file

if __name__ == '__main__':
    plotter = Plotter()
    plotter.run(sys.argv[1] if len(sys.argv) > 1 else plotter.get_latest_file(DATA_FOLDER_PATH))