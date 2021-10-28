from flask import Flask, render_template, request, jsonify
import os
import requests
from datetime import timedelta, time, datetime
from itertools import islice
import re

app = Flask(__name__)

# Route for "/" (frontend):
@app.route('/')
def index():
  return render_template("index.html")


# Route for "/weather" (middleware):
@app.route('/weather', methods=["POST"])
def POST_weather():
  course = request.form["course"]
  print("course entered is:" , course)

  # Parsing course input
  try:
    subject, number = course.split(" ")     
  except:  #check for edge cases: what if no spaces
    parsed_course = re.split(r'(^[^\d]+)', course)[1:]
    subject, number = parsed_course[0], parsed_course[1]

  # Retrieving course data
  server_url = os.getenv('COURSES_MICROSERVICE_URL') 
  r = requests.get(f'{server_url}/{subject}/{number}/')
  course_data = r.json()
  print(course_data)  

  try:
  #Setting up class schedule to calculate
    course_days = course_data["Days of Week"]
    list_meeting = [0, 0, 0, 0, 0, 0, 0] # include weekends
    for i in range(len(course_days)):
      day = course_days[i:i+1]
      if day == "M":
        list_meeting[0] = 1
      elif day == "T":
        list_meeting[1] = 1
      elif day == "W":
        list_meeting[2] = 1
      elif day == "R":
        list_meeting[3] = 1
      elif day == "F":
        list_meeting[4] = 1
  except:
    print("EXCEPT")
    print(course_data["error"])
    print(course_data["course"])
    return course_data["error"], 404

  print(list_meeting)
  course_time = course_data["Start Time"]
  print("start time is: ")
  print(course_time.split())
  course_time_stats = course_time.split()
  course_time = datetime.strptime(course_time_stats[0], "%H:%M")
  course_time = course_time.time()

  if course_time_stats[1] == "PM" and "12" not in str(course_time_stats[0]):
    print("afternoon")
    #Creating datetime object in order to convert regular to army time
    dt = datetime(2021, 1, 10)
    combined = dt.combine(dt, course_time)
    print(combined)
    course_time_temp = combined + timedelta(hours = 12)
    course_time =course_time_temp.time()
    print(course_time)
    
  print(course_time, type(course_time))

  current_datetime = datetime.now()
  current_date = current_datetime.date()
  current_time = current_datetime.time()
  current_weekday = current_datetime.weekday()
  
  # print(current_date)
  # print(current_time)
  # print(course_time < current_time)
  # print(current_weekday)

  # #finding next day of class 
  next_week = False
  for i in range(current_weekday, current_weekday + 7):
    current = i % 7
    print(i, current)
    if i == 6:
      next_week = True
    if list_meeting[current] == 1:
      print("found match")
      if current_weekday == current:
        if current_time < course_time:
          print("next meeting time:", current)
          print(current)
          break
      elif current_weekday < i: #changed current to i
          print("next meeting time:", current)
          print(current)
          break

  diff_days = current - current_weekday
  next_course_date = current_date + timedelta(diff_days)
  if next_week == True:
    next_course_date = next_course_date + timedelta(7)
  print(next_course_date)
  nextCourseMeeting = str(next_course_date) + " " +str(course_time)
  # print(course)
  # print(nextCourseMeeting)

 # Weather API              https://api.weather.gov/gridpoints/ILX/95,71/forecast/hourly
  w = requests.get("https://api.weather.gov/gridpoints/ILX/95,71/forecast/hourly")
  weather_data = w.json()
  properties_list = weather_data["properties"]["periods"]

  print(course_time)
  weather_course_time = course_time.replace(microsecond=0, second=0, minute=0) #round down the time
  print(weather_course_time)

  temperature = -1
  shortForecast = "Sunny"
  for properties in properties_list:
    if str(weather_course_time) in str(properties["startTime"]) and str(next_course_date) in str(properties["startTime"]):
      print(properties["startTime"], properties["temperature"], properties["shortForecast"])
      temperature = int(properties["temperature"])
      shortForecast = str(properties["shortForecast"])
      # print("temperature is:", temperature)
      # print("forecast:", shortForecast)

  forecastTime = str(next_course_date) + " " + str(weather_course_time)

  return jsonify({"course": course,  #check if needed to display on frontend
                  "nextCourseMeeting": nextCourseMeeting, 
                  "forecastTime": forecastTime,
                  "temperature": temperature,
                  "shortForecast": shortForecast}), 200


