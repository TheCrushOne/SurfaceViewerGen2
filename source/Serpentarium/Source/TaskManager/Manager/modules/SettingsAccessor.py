import os
import json
import logging
import jsonmerge

import LoggerAccessor

SETTINGS_PATH_DEFAULT = os.path.abspath(os.path.join(os.path.dirname(__file__), 'settings.json'))

try:
   from django.conf import settings

   SETTINGS_PATH = settings.SETTINGS_PATH if 'DJANGO_SETTINGS_MODULE' in os.environ else SETTINGS_PATH_DEFAULT

except (ImportError, AttributeError):
   SETTINGS_PATH = SETTINGS_PATH_DEFAULT


KEY_TAG = 'key'
DEF_TAG = 'def'
ENV_TAG = 'env'

EMPLACED_VARS_TAG = 'emplaced_vars'


logger = LoggerAccessor.get('settings_accessor')


def get(path, customSettingsPath=None):
   """ Path - setting node path separated by '|' """
   settingsDef = __load(customSettingsPath if customSettingsPath else SETTINGS_PATH_DEFAULT)
   settingsUser = __load(SETTINGS_PATH)

   settings = jsonmerge.merge(settingsDef, settingsUser)

   node = __find(settings, path)
   val = __get_val(node, path)
   if val:
      val = __apply_emplaced_vars(settings, val)
   return val


def __get_val(node, path=None):
   if not node:
      logger.error(f"Node '{path}' doesn't exist")
      return None

   if not DEF_TAG in node:
      logger.error(f"Node '{path}' default value doesn't exist")
      return None

   return os.environ.get(node[ENV_TAG], node[DEF_TAG]) if ENV_TAG in node else node[DEF_TAG] 


def __find(settings, path):
   node = settings
   for key in path.split('|'):
      if not key in node:
         return None
      node = node[key]
   return node


def __load(settingsPath):
   try:
      with open(settingsPath, 'r') as settingsFile:
         return json.load(settingsFile)
   except Exception:
      logger.exception(f"Failed to load settings file '{settingsPath}'")
   return {}


def __apply_emplaced_vars(settings, val):
   if not isinstance(val, str):
      logger.debug(f"Can't apply emplaced variables to node value '{val}' because it isn't string")
      return val

   emplVars = __find(settings, EMPLACED_VARS_TAG)
   if not emplVars:
      return val

   for emplVar in emplVars:
      val = val.replace(emplVar[KEY_TAG], __get_val(emplVar))
   return val
