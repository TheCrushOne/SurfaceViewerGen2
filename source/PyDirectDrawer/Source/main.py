import numpy as np
import matplotlib.dates as mdates

#from matplotlib.patches import StepPatch
import pandas as pd
import json
import sys
import os
import glob
#from adjustText import adjust_text

sys.path.append(os.path.dirname(__file__))

from data_processing.gantt_boxes import GanttBoxProvider
from data_processing.readiness_stairs import ReadinessStairsProvider
from drawing.matplotdrawer import DrawingProvider
from drawing.draw_type import DrawType

DATA_FILE_PATH = "E:\\FilesX\\PrOgRaMmInG\\2019_2020_9th_course\\SurfaceViewerGen2\\cache\\gantt_demo\\research\\17032021_222427_universal.rres"
#DATA_FILE_PATH = "C:\\Projects\\surfaceviewergen2\\cache\\gantt_demo\\research\\13032021_194624_universal.rres"
DATA_FOLDER_PATH = "..\\..\\cache\\gantt_demo\\research\\"

class Plotter:
    gantt_box_provider = {}
    readiness_stairs_provider = {}

    drawing_provider = {}

    unit_count = 0
    label_pool = []
    vlines_cluster = dict()

    #draw_type = DrawType.DT_SMSTAIRS
    #draw_type = DrawType.DT_SPLITTED
    draw_type = DrawType.DT_AFTERWARDS

    def __init__(self):
        self.gantt_box_provider = GanttBoxProvider()
        self.readiness_stairs_provider = ReadinessStairsProvider()
        self.drawing_provider = DrawingProvider()
           
    def run(self, data_file_path):
        with open(data_file_path) as json_file:
            data = json.load(json_file)
            self.gantt_box_provider.prepare_data(data)
            self.readiness_stairs_provider.prepare_data(data)
            self.drawing_provider.draw(
                self.draw_type,
                self.gantt_box_provider.get_gantt_data(),
                self.gantt_box_provider.get_delimiters_data(),
                self.readiness_stairs_provider.get_stairs_data(),
                self.readiness_stairs_provider.get_time_stairs_data()
            )
            
    def get_latest_file(self, path):
        list_of_files = glob.glob(os.path.join(path, "*.rres"))
        latest_file = max(list_of_files, key=os.path.getctime)
        return latest_file

if __name__ == '__main__':
    plotter = Plotter()
    #plotter.run(sys.argv[1] if len(sys.argv) > 1 else plotter.get_latest_file(DATA_FOLDER_PATH))
    plotter.run(DATA_FILE_PATH)