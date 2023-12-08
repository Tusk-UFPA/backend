#! /bin/bash

python -m venv venv 
source venv/bin/activate
python  pip install --upgrade pip
python  pip install -r requirements.txt

python SolarTracking.py