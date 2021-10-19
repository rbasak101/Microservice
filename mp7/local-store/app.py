from flask import Flask, render_template, request, jsonify
import requests
app = Flask(__name__)

# TODO:
dictionary = {} # stores key as version
# PUT /<key> – Adds a versioned object
@app.route('/<key>', methods=["PUT"])
def add(key):
    # if request.method == "PUT":
    version = len(dictionary) + 1
    value = request.data.decode("utf-8")

    dictionary[key][version] = value
    print("Hello")
    return "Added", 200

#GET /<key> – Retrieves the latest version of a key
@app.route("/<key>", methods=["GET"])
def retrieve_latest(key): # return the highest number or last in dictionary
    if len(dictionary) == 0:
        return "Empty", 404
    if key not in dictionary:
        return "key not present", 404
    #max_key = max(dictionary, key = dictionary.get)
    return jsonify({"value: " : dictionary[key][len(dictionary[key])], "version: " :len(dictionary[key])}), 200

# # GET /<key>/<version> – Retrieves a specific version of a key
@app.route("/<key>/<version>", methods=["GET"])
def specific_retrieve(key, version):
    if key not in dictionary:
        return "key not present", 404
    return jsonify({"value: ": dictionary[key][version], "version: " : version }), 200
    #pass

# DELETE /<key> – Completely deletes a key
@app.route("/<key>", methods=["DELETE"])
def delete(key):
    if key not in dictionary:
        return "key not present", 404
    del dictionary[key]
#     return "Deleted", 200
