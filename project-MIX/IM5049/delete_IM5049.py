from flask import Flask, jsonify, request
import requests

class_middleware = "http://cs240-adm-01.cs.illinois.edu:5000/microservice"

my_ip = "http://172.22.150.7"

data = {
    "port" : "5049",
    "ip" : my_ip,
}

status = requests.delete(class_middleware, json = data)
print(status)
print("Finished")