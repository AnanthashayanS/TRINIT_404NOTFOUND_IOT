# -*- coding: utf-8 -*-
"""Backend.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1JBo9dTE1QjJoFkCauvseDgeNzGzG0OUl
"""

!pip install Pyrebase
!pip install haversine
!pip install requests
!pip install paho-mqtt

import pyrebase
import time
import requests
import haversine as hs
import paho.mqtt.publish as publish
import numpy as np
import pandas as pd
from sklearn import linear_model
from sklearn import metrics
import matplotlib.pyplot as plt
from sklearn.metrics import precision_score

def driver_assistance_system(speed, acceleration, idling_time, braking_count, obstacle_detected):
    if speed > 90:
        print("Reduce your speed, the recommended cruising speed is 90 km/h.")
    elif acceleration > 1.4705:
        print("Slow down, the recommended maximum acceleration is 1.4705 m/s2.")
    elif idling_time > 30:
        print("It is recommended to switch off the engine as it has been idling for more than 30 seconds.")
    elif braking_count > 3:
        print("Try to reduce the number of brakes, excessive braking can affect the vehicle's performance.")
    elif obstacle_detected:
        print("An obstacle has been detected ahead, it is recommended to decelerate.")
    else:
      print("You are Safe ,Continue your Journey")

x1 = np.array([75.2,52.4,63.6,95.2,100.2,82.9,61.6,64.5,21.0,22.4,23.9,42.5,33.0,35.9,32.3,33.6,45.9,43.3,45.4,39.3,56.4,59.2,37.7,41.5,52.1,54.3,49.1,48.7,56.0,51.8])
x2 = np.array([42.4,20.4,43.4,101.2,71.6,114.7,15.0,14.7,5.7,5.2,5.7,10.7,10.0,10.4,10.0,10.6,18.0,18.0,18.0,18.0,26.2,20.4,11.5,11.0,11.0,11.0,11.0,11.0,11.0,11.0])
y= [4.83,2.42,5.85,14.89,10.94,15.00,1.68,1.58,0.69,0.66,0.72,1.21,1.09,1.16,1.03,1.09,1.92,1.87,2.13,2.06,3.45,2.42,1.21,1.20,1.15,1.18,1.10,1.07,1.12,1.04]
X= np.stack((x2,x1),axis=1)
regr1 = linear_model.LinearRegression()
regr1.fit(X,y)

firebaseConfig = {"apiKey": "AIzaSyBtvYVGOK_mABhU41riPue9cQicjnWvoIU",
  "authDomain": "driverassisting.firebaseapp.com",
  "databaseURL": "https://driverassisting-default-rtdb.firebaseio.com",
  "projectId": "driverassisting",
  "storageBucket": "driverassisting.appspot.com",
  "messagingSenderId": "939478415947",
  "appId": "1:939478415947:web:0bbfb008e8b8e20f5aabce",
  "measurementId": "G-1N5LZWKHHZ"}

firebase = pyrebase.initialize_app(firebaseConfig)
db = firebase.database()

# Example usage
current_speed = int(db.child("Sensor_Out").child("current_speed").get().val())
current_acceleration = float(db.child("Sensor_Out").child("current_acceleration").get().val())
idling_duration = int(db.child("Sensor_Out").child("idling_duration").get().val())
number_of_brakes = int(db.child("Sensor_Out").child("number_of_brakes").get().val())
obstacle_present = str(db.child("Sensor_Out").child("obstacle_present").get().val())

source_location = (db.child("Sensor_Out").child("source_location").get().val())
Dest_location = (db.child("Sensor_Out").child("source_location").get().val())

a=hs.haversine(source_location,Dest_location)
pow1 = regr1.predict([[a,current_speed]])

output=driver_assistance_system(current_speed, current_acceleration, idling_duration, number_of_brakes, obstacle_present)
print(*"%.2f"%abs(pow1),"Litre/Km for your Travel")