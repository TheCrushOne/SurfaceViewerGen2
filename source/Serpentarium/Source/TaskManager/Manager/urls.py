from django.urls import include, path
from django.views.decorators.csrf import csrf_exempt
from rest_framework import routers

from Manager.app import builder_api, tasks_api, experimental_api, logs_api


router = routers.DefaultRouter()


urlpatterns = [
    #NOTE: system
    path('', include(router.urls)),

    #NOTE: log viewer
    path('api/logs/app', csrf_exempt(logs_api.logs_app)),
    path('api/logs/wsgi', csrf_exempt(logs_api.logs_wsgi)),

    #NOTE: status
    path('api/tasks', csrf_exempt(tasks_api.get_tasks)),
    path('api/tasks/<uuid:guid>', csrf_exempt(tasks_api.get_task)),
    path('api/tasks/<uuid:guid>/output', csrf_exempt(tasks_api.get_task_output)),
    path('api/tasks/running', csrf_exempt(tasks_api.get_running_task)),

    #NOTE: control
    path('api/tasks/cancel', csrf_exempt(tasks_api.cancel)),

    #NOTE: build commands
    path('api/build/chartcache/<str:target>', csrf_exempt(builder_api.chartcache)),
    path('api/build/path/<str:target>', csrf_exempt(builder_api.path)),

    #NOTE: experimental
    path('api/experimental/pack_hound', csrf_exempt(experimental_api.pack_hound))
]
