import os
import json
import requests

import SettingsAccessor, LoggerAccessor

TASK_EXECUTOR_URL = SettingsAccessor.get('task_executor|url')
PYTHON_PATH = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(__file__)))), r'Scripts\python.exe')

logger = LoggerAccessor.get('tasks_manager')


class TaskInfo:
   def __init__(self, name, tag, exec, args):
      self.name = name
      self.tag  = tag
      self.exec = exec
      self.args = args

      self.guid = None
      self.stat = None

   def __str__(self):
      mainStr = f"'{self.name}'"
      if not self.tag and not self.guid:
         return mainStr
      infoStr = ', '.join(filter(None, [self.tag, self.guid]))
      return mainStr + f" ({infoStr})"


class AddTaskException(Exception):
   def __init__(self, task, excepion_msg):
      assert isinstance(task, TaskInfo)
      self.task = task
      self.excepion_msg = excepion_msg

   def __str__(self):
      return f"Adding task {str(self.task)} failed!\n{self.excepion_msg}"


def add_task(name, exec, args='', tag=None):
   task = TaskInfo(name, tag, exec, args)
   try:
      r = requests.post(url=f'{TASK_EXECUTOR_URL}/tasks', json=vars(task))
      if r.status_code == 200:
         return r.json()
   except Exception as e:
      exc = AddTaskException(task, e)
      logger.exception(str(exc))
      raise exc
   return None


def add_py_task(name, file, args='', tag=None):
   assert os.path.exists(PYTHON_PATH)
   p = os.path.abspath(file)
   args.insert(0, p)
   return add_task(name, PYTHON_PATH, args, tag)


def get_tasks(tag=None):
   try:
      r = requests.get(url=f'{TASK_EXECUTOR_URL}/tasks?tag={tag}' if tag else f'{TASK_EXECUTOR_URL}/tasks')
      if r.status_code == 200:
         return r.json()
      logger.error(f"Failed to get tasks! Status code {r.status_code}")
   except Exception:
      logger.exception(f"Failed to get tasks!")
   return None


def get_task(guid):
   try:
      r = requests.get(url=f'{TASK_EXECUTOR_URL}/tasks/{guid}')
      if r.status_code == 200:
         return r.json()
      logger.error(f"Failed to get task '{guid}'! Status code {r.status_code}")
   except Exception:
      logger.exception(f"Failed to get task '{guid}'!")
   return None


def get_task_output(guid):
   try:
      r = requests.get(url=f'{TASK_EXECUTOR_URL}/tasks/{guid}/output')
      if r.status_code == 200:
         return r.json()
      logger.error(f"Failed to get task output '{guid}'! Status code {r.status_code}")
   except Exception:
      logger.exception(f"Failed to get task output '{guid}'!")
   return None


def get_running_task():
   try:
      r = requests.get(url=f'{TASK_EXECUTOR_URL}/tasks/running')
      if r.status_code == 200:
         return r.json()
      logger.error(f"Failed to get running task! Status code {r.status_code}")
   except Exception:
      logger.exception(f"Failed to get running task!")
   return None


def cancel(guid=None, tag=None):
   """ Params should contain tag OR guid """
   if (guid and tag) or (not guid and not tag):
      return False

   try:
      r = requests.post(url=f'{TASK_EXECUTOR_URL}/tasks/cancel', data=json.dumps({'tag': tag} if tag else {'guid' : guid}), headers={'content-type': 'application/json'})
      if r.status_code == 200:
         return r.json()
      logger.error(f"Failed to cancel task! Status code {r.status_code}")
   except Exception:
      logger.exception(f"Failed to cancel task!")
   return False


def prepare_json_arg(jsonData):
   idx = json.dumps(jsonData).replace("\"", "\\\"")
   return f"\"{idx}\""
