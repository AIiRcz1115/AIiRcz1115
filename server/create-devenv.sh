#!/bin/bash
#Ubuntu 14.04

sudo apt-get update

sudo apt-get install python3-pip
sudo pip3 install virtualenv
virtualenv server/venv
source server/venv/bin/activate
pip3 install django
django-admin --version
pip3 install django-registration-redux
deactivate
cd AIiRcz1115

exit 0
