import os
import sys
import logging

from logging.handlers import RotatingFileHandler

try:
   from django.conf import settings

   IS_DJANGO_PROD = not settings.DEBUG if 'DJANGO_SETTINGS_MODULE' in os.environ else False

except ImportError:
   IS_DJANGO_PROD = False

try:
   from opencensus.ext.azure.log_exporter import AzureLogHandler

   OPENCENSUS_KEY = os.environ.get('OPENCENSUS_KEY', None)
except ImportError:
   OPENCENSUS_KEY = None

DJANGO_LOG = os.environ.get('DJANGO_LOG', None)

STANDARD_FMT   = '[%(asctime)s] %(levelname)s [%(name)s:%(lineno)s] %(message)s'
OPENCENSUS_FMT = '%(levelname)s [%(name)s:%(lineno)s] %(message)s'

DATE_FMT = '%Y-%m-%d %H:%M:%S'


standard_formatter   = logging.Formatter(STANDARD_FMT,   DATE_FMT)
opencensus_formatter = logging.Formatter(OPENCENSUS_FMT, DATE_FMT)

stdout_handler = logging.StreamHandler(sys.stdout)
stdout_handler.setFormatter(standard_formatter)

# Rotating log file handler
file_handler = None

# Azure Application Insights log handler
azure_handler = None

def get_fle_handler(filename):
   global file_handler
   if file_handler:
      return file_handler
   file_handler = RotatingFileHandler(filename, maxBytes=1024*1024, backupCount=5)
   return file_handler


def get_azure_handler(appkey):
   global azure_handler
   if azure_handler:
      return azure_handler
   azure_handler = AzureLogHandler(connection_string=f'InstrumentationKey={appkey}')
   return azure_handler


def get(name, lvl=logging.WARNING):
   logger = logging.getLogger(name)
   logger.handlers.clear()

   logger.addHandler(stdout_handler)

   if IS_DJANGO_PROD:
      if DJANGO_LOG:
         h = get_fle_handler(DJANGO_LOG)
         h.setFormatter(standard_formatter)
         logger.addHandler(h)

   if OPENCENSUS_KEY:
      h = get_azure_handler(OPENCENSUS_KEY)
      h.setFormatter(opencensus_formatter)
      logger.addHandler(h)

   logger.setLevel(lvl)

   return logger
