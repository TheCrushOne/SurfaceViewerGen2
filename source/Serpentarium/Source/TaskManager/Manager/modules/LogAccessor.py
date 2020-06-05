import os
import logging

import SettingsAccessor, LoggerAccessor

APP_LOG_PATH  = SettingsAccessor.get('logs_paths|app')
WSGI_LOG_PATH = SettingsAccessor.get('logs_paths|wsgi')


logger = LoggerAccessor.get('logs_assessor')


def get_app_log(linesNum = 1000):
   return __get_lines_from_file(APP_LOG_PATH, linesNum)


def get_wsgi_log(linesNum = 1000):
   return __get_lines_from_file(WSGI_LOG_PATH, linesNum)


def __get_lines_from_file(path, linesNum):
   lines = []

   if not path:
      return lines

   try:
      with open(path, 'r') as f:
         lines = f.readlines()
   except Exception as e:
      logger.exception(f"Logs assessor: getting log from '{path}' failed. Exception: {e}")
   return lines[-linesNum:]
