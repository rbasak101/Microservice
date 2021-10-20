from flask import Flask, render_template, request, jsonify
import requests
app = Flask(__name__)

# TODO:
dictionary = {} # stores key as version
# PUT /<key> – Adds a versioned object
@app.route("/<key>", methods=["PUT"])
def add(key):
    # if request.method == "PUT":
    value = request.data.decode("utf-8")
    if key not in dictionary:
        dictionary[key] = {}
    version = len(dictionary[key]) + 1
    dictionary[key][version] = value
    # print(dictionary)
    return "Added", 200

# #GET /<key> – Retrieves the latest version of a key
@app.route("/<key>", methods=["GET"])
def retrieve_latest(key): 
    if len(dictionary) == 0:
        return "Empty", 404
    if key not in dictionary:
        return "Key not present", 404
    #max_key = max(dictionary, key = dictionary.get)
    return jsonify({"value" : dictionary[key][len(dictionary[key])], "version" :len(dictionary[key])}), 200

# # # GET /<key>/<version> – Retrieves a specific version of a key
@app.route("/<key>/<version>", methods=["GET"])
def specific_retrieve(key, version):
    if key not in dictionary:
        return "Key not present", 404
    print("printing version number right now...")
    print(version)
    print(type(version)) #version is a string
    if(int(version) > len(dictionary[key]) or int(version) < 1):
        return "version is out of bounds", 404
    return jsonify({"value": dictionary[key][int(version)], "version" : version }), 200


# DELETE /<key> – Completely deletes a key
@app.route("/<key>", methods=["DELETE"])
def delete(key):
    if key not in dictionary:
        return "Key not present", 404
    del dictionary[key]
    # print(dictionary)
    return "Deleted", 200
