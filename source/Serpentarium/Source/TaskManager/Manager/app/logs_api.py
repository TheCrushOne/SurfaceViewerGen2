from django.shortcuts import HttpResponse
from rest_framework.decorators import api_view

from Manager.modules import LogAccessor

@api_view(['GET'])
def logs_app(request):
   return HttpResponse(LogAccessor.get_app_log(), content_type="text/plain")


@api_view(['GET'])
def logs_wsgi(request):
   return HttpResponse(LogAccessor.get_wsgi_log(), content_type="text/plain")
