from django.conf.urls import url

from . import views

urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'^$', views.index, name='index'),
    url(r'^test_primality/', views.run_primality_test, name='run_primality_test'),
    url(r'^stop_task/(?P<task_id>[0-9a-z]+)/$', views.stop_task, name='stop_task')
]
