from django.conf import settings
from django.http import JsonResponse, HttpResponse, HttpResponseBadRequest
from rest_framework.decorators import api_view

import json
import logging

from Manager.modules import TaskManager, LoggerAccessor

logger = LoggerAccessor.get('tasks_api')

@api_view(['POST'])
def get_tasks(request):
   return JsonResponse(TaskManager.get_tasks(request.GET.get('tag', None)), safe=False)


@api_view(['GET'])
def get_task(request, guid):
   return JsonResponse(TaskManager.get_task(guid), safe=False)


@api_view(['GET'])
def get_task_output(request, guid):
   return HttpResponse(TaskManager.get_task_output(guid), content_type="text/plain")


@api_view(['GET'])
def get_running_task(request):
   task = TaskManager.get_running_task()
   return JsonResponse(task, safe=False) if task else HttpResponse(status=204)


@api_view(['POST'])
def cancel(request):
   params = json.loads(request.body or '{}')
   if (not 'tag' in params and not 'guid' in params) or ('tag' in params and 'guid' in params):
      return HttpResponseBadRequest("Task tag OR guid required")
   return HttpResponse() if TaskManager.cancel(params) else HttpResponse(status=204)
