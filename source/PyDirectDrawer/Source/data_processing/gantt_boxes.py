import matplotlib.colors as mcolors
import sys
import os

sys.path.append(os.path.dirname(__file__))

from common_base.log_value_token import LogValueToken
from common_base.log_level_token import LogLevelToken

class Shard():
    idx = 0
    color = ''
    interval = (0, 0)
    prefix = ''
    unit_idx = 0
    shard_idx = 0

    def __init__(self, idx, color, interval, prefix, unit_idx, shard_idx):
        self.idx = idx
        self.color = color
        self.interval = interval
        self.prefix = prefix
        self.unit_idx = unit_idx
        self.shard_idx = shard_idx

class GanttBoxProvider():
    shard_data = []
    color_palette = []

    current_shard_data = []

    label_pool = []

    def allocate_color_palette(self, count):
        color_names = list(mcolors.CSS4_COLORS)
        return color_names    

    def parse_task_log(self, hrd):
        if hrd[LogValueToken.LVT_UNIT_IDX] in self.label_pool:
            prefix = '_'
        else:
            self.label_pool.append(hrd[LogValueToken.LVT_UNIT_IDX])
            prefix = ''
        shard = Shard(
            idx = hrd[LogValueToken.LVT_HOLDER_IDX],
            color = self.color_palette[hrd[LogValueToken.LVT_UNIT_IDX]],
            interval = (hrd[LogValueToken.LVT_START_TS], hrd[LogValueToken.LVT_FINISH_TS] - hrd[LogValueToken.LVT_START_TS]),
            prefix = prefix,
            unit_idx = hrd[LogValueToken.LVT_UNIT_IDX],
            shard_idx = hrd[LogValueToken.LVT_SHARD_IDX],
        )
        self.current_shard_data.append(shard)
        #self.plot_single_shard(hrd)

    def parse_packet_log(self, hcrd):
        for shard in hcrd[LogLevelToken.LLT_SUB_DATA]: 
            self.parse_task_log(shard[LogLevelToken.LLT_TASK])

    #def draw_vlines(self, hcrd):
        #self.vlines_cluster

    def parse_run_log(self, hcrh):
        self.unit_count = hcrh[LogValueToken.LVT_UNIT_COUNT]
        #self.analyze_shard_count(hcrh)
        for shard in hcrh[LogLevelToken.LLT_SUB_DATA]:
            self.color_palette = self.allocate_color_palette(hcrh[LogValueToken.LVT_UNIT_COUNT])            
            self.parse_packet_log(shard[LogLevelToken.LLT_PACKET])
            #self.draw_vlines()
            

    def parse_pipeline_log(self, exh):
        assert(len(exh[LogLevelToken.LLT_SUB_HISTORY]) == 1)
        for shard in exh[LogLevelToken.LLT_SUB_HISTORY]:
            self.parse_run_log(shard[LogLevelToken.LLT_RUN])

    def parse_overall_log(self, exh):
        for shard in exh[LogLevelToken.LLT_SUB_HISTORY]:
            self.shard_data.append([])
            self.current_shard_data = self.shard_data[-1]
            #self.plot_gantt_chart(shard[LogLevelToken.LLT_PIPELINE])
            self.parse_pipeline_log(shard[LogLevelToken.LLT_PIPELINE])

    def prepare_data(self, data):
        self.parse_overall_log(data[LogLevelToken.LLT_OVERALL])

    def get_gantt_data(self):
        return self.shard_data