import sys
import os
import numpy as np

sys.path.append(os.path.dirname(__file__))

from common_base.log_value_token import LogValueToken
from common_base.log_level_token import LogLevelToken
from readiness_data import ReadinessData

class ReadinessStairsProvider():
    shard_count = []
    shard_status = []
    shard_data = []
    shard_timed_data = []

    current_shard_data = []

    current_shard_timed_data = None

    def init_shard_status(self, hcrh):
        self.unit_count = hcrh[LogValueToken.LVT_UNIT_COUNT]
        self.current_shard_timed_data.unit_count = self.unit_count
        self.current_shard_data.unit_count = self.unit_count

        self.shard_count = np.zeros(self.unit_count)
        #self.shard_data = []
        mmax = 0
        for rshard in hcrh[LogLevelToken.LLT_SUB_DATA]:
            hcrd = rshard[LogLevelToken.LLT_PACKET]
            for pshard in hcrd[LogLevelToken.LLT_SUB_DATA]: 
                task = pshard[LogLevelToken.LLT_TASK]
                if (self.shard_count[task[LogValueToken.LVT_UNIT_IDX]] < task[LogValueToken.LVT_SHARD_IDX] + 1):
                    self.shard_count[task[LogValueToken.LVT_UNIT_IDX]] = task[LogValueToken.LVT_SHARD_IDX] + 1
                else:
                    self.shard_count[task[LogValueToken.LVT_UNIT_IDX]]
                mmax = task[LogValueToken.LVT_SHARD_IDX] + 1 if (mmax < task[LogValueToken.LVT_SHARD_IDX] + 1) else mmax
                    
        self.shard_status = np.zeros((self.unit_count, mmax))

    def analyze_shard_count(self, hcrh):
        self.init_shard_status(hcrh)

        shard_readiness_main = self.current_shard_data.y
        time_readiness = self.current_shard_timed_data.y
        time_stamps = self.current_shard_timed_data.x

        current_finished = 0
        shard_readiness_main.append(current_finished)
        for rshard in hcrh[LogLevelToken.LLT_SUB_DATA]:
            hcrd = rshard[LogLevelToken.LLT_PACKET]
            for pshard in hcrd[LogLevelToken.LLT_SUB_DATA]: 
                task = pshard[LogLevelToken.LLT_TASK]
                self.shard_status[task[LogValueToken.LVT_UNIT_IDX]][task[LogValueToken.LVT_SHARD_IDX]] = 1
                if (self.shard_status[task[LogValueToken.LVT_UNIT_IDX]].tolist().count(1) == self.shard_count[task[LogValueToken.LVT_UNIT_IDX]]):
                    current_finished += 1
            shard_readiness_main.append(current_finished)
            time_readiness.append(current_finished)
            time_stamps.append(hcrd[LogValueToken.LVT_FINISH_TS])

    def parse_pipeline_log(self, exh):
        assert(len(exh[LogLevelToken.LLT_SUB_HISTORY]) == 1)
        self.current_shard_timed_data.packet_size = exh[LogValueToken.LVT_PACKET_SIZE]
        self.current_shard_timed_data.thread_count = exh[LogValueToken.LVT_THREAD_COUNT]
        self.current_shard_timed_data.split_factor = exh[LogValueToken.LVT_SPLIT_FACTOR]
        self.current_shard_data.packet_size = exh[LogValueToken.LVT_PACKET_SIZE]
        self.current_shard_data.thread_count = exh[LogValueToken.LVT_THREAD_COUNT]
        self.current_shard_data.split_factor = exh[LogValueToken.LVT_SPLIT_FACTOR]
        for shard in exh[LogLevelToken.LLT_SUB_HISTORY]:
            self.analyze_shard_count(shard[LogLevelToken.LLT_RUN])

    def parse_overall_log(self, exh):
        for shard in exh[LogLevelToken.LLT_SUB_HISTORY]:
            self.shard_data.append(ReadinessData())
            self.shard_timed_data.append(ReadinessData())
            self.current_shard_data = self.shard_data[-1]
            self.current_shard_timed_data = self.shard_timed_data[-1]
            self.parse_pipeline_log(shard[LogLevelToken.LLT_PIPELINE])
            self.current_shard_data.x = np.arange(len(self.current_shard_data.y))

    def prepare_data(self, data):
        self.parse_overall_log(data[LogLevelToken.LLT_OVERALL])

    def get_stairs_data(self):
        return self.shard_data
    
    def get_time_stairs_data(self):
        return self.shard_timed_data